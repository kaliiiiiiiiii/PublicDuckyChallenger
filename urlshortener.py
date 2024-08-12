import argparse
import asyncio
import aiohttp
import itertools
from urllib import parse
import json

good_chars = (  # characters which are in the same place for different keyboard layouts
    "t" "uiop"
    "asdfghjkl"
    "xcvbnm"
)
good_chars = [*good_chars]


def char_gen(_range: range = range(5, 10)):
    # generates A, B, C... to ZZZZ for the length
    for _length in _range:
        for chars in itertools.permutations(good_chars, r=_length):
            string = "".join(chars)
            yield string


async def check_url(key: str):
    # checks if a key for the short url is already taken
    # returns:
    # not taken:    (key, 1)
    # taken:        ([original_url, shorturl], 2)
    # flagged:      ("", 3)

    url = f"https://is.gd/{key}"
    async with aiohttp.request("GET", url, allow_redirects=False) as resp:
        if resp.status == 404:
            # not taken
            return key, 1
        elif resp.status == 301:
            loc = resp.headers.get("Location", "")
            taken = 2
            if loc == "":
                taken = 3
            # taken
            return [loc, url], taken
        elif resp.status in [200, 410]:
            # flagged
            return "", 3
        else:
            raise ValueError(f"got unexpected response status: {resp.status} for {url}")


async def create(url: str, key: str) -> str:
    # create short url based on key and original url
    params = {"format": "json", 'shorturl': key, "url": url}
    url = "https://is.gd/create.php?" + parse.urlencode(params)
    async with aiohttp.request("GET", url, allow_redirects=False) as resp:
        assert resp.status == 200
        return json.loads(await resp.text())["shorturl"]


async def find_shortest_ok(url, _range: range = range(5, 10)):
    # finds shortest url (for generated character keys) which isn't taken ot matches the original url
    # returns:
    # not taken:            (key, False)
    # matches original url: (shorturl, True)
    gen = char_gen(_range)
    stop = False
    while not stop:
        futs = []
        # https://is.gd/terms.php
        # > Users may not open more than 5 concurrent connections to is.gd at any time
        for _ in range(5):
            try:
                futs.append(check_url(gen.__next__()))
            except StopIteration:
                stop = True
                break
        for res, taken in await asyncio.gather(*futs):
            if taken == 1:
                return res, False
            elif taken == 2:
                got_url, short = res
                if got_url == url:
                    return short, True
    raise


async def create_short(url: str, _range: range = range(5, 10)):
    key_or_url, is_ok = await find_shortest_ok(url, _range)
    if is_ok:
        return key_or_url
    return await create(url, key_or_url)


async def main(url):
    _range = range(5, 10)
    data = await create_short(url, _range)
    print(data)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("url", help="The url to shorten", type=str)
    args = parser.parse_args()
    asyncio.run(main(args.url))
