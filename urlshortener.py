import argparse
import asyncio
import aiohttp
import itertools
from urllib import parse
import json

good_chars = (
    "t" "uiop"
    "asdfghjkl"
    "xcvbnm"

)
good_chars = [*good_chars]


def urls_gen(_range: range = range(5, 10)):
    for _length in _range:
        for chars in itertools.permutations(good_chars, r=_length):
            string = "".join(chars)
            yield string


async def check_url(key: str):
    url = f"https://is.gd/{key}"
    async with aiohttp.request("GET", url, allow_redirects=False) as resp:
        if resp.status == 404:
            return key
        elif resp.status in [301, 200, 410]:
            return False
        else:
            raise ValueError(f"got unexpected response status: {resp.status} for {url}")


async def create(url: str, key:str) -> str:
    params = {"format": "json", 'shorturl':key, "url":url}
    url = "https://is.gd/create.php?" + parse.urlencode(params)
    async with aiohttp.request("GET", url, allow_redirects=False) as resp:
        assert resp.status == 200
        return json.loads(await resp.text())["shorturl"]


async def find_shortest_ok(_range: range = range(5, 10)):
    gen = urls_gen(_range)
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
        for res in await asyncio.gather(*futs):
            if res:
                return res
    raise 


async def create_short(url: str,_range: range = range(5, 10)):
    key = await find_shortest_ok(_range)
    return await create(url, key)


async def main(url):
    _range = range(5, 10)
    data = await create_short(url, _range)
    print(data)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("url", help="The url to shorten", type=str)
    args = parser.parse_args()
    asyncio.run(main(args.url))
