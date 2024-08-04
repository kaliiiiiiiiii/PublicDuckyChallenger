import asyncio
import os
import argparse

from selenium_driverless import webdriver
from selenium_driverless.types.by import By
from selenium_driverless.types.webelement import NoSuchElementException


def find_edge_path() -> str:
    for item in map(
            os.environ.get,
            ("PROGRAMFILES", "PROGRAMFILES(X86)", "PROGRAMW6432"),
    ):
        if item is not None:
            path = os.sep.join((item, "Microsoft/Edge/Application", "msedge.exe"))
            if os.path.isfile(path) and os.access(path, os.X_OK):
                return os.path.normpath(path)
    raise FileNotFoundError("Couldn't find installed MSEdge executable")


async def write_email(to: str, betreff: str, content: str, cc: str = None, headless: bool = True):
    options = webdriver.ChromeOptions()
    if headless:
        options.headless = True
    options.binary_location = find_edge_path()
    options.add_argument("--new-window")
    async with webdriver.Chrome(options) as driver:
        await asyncio.sleep(5)
        for _target in (await driver.targets).values():
            if _target.url == "about:blank":
                target = _target.Target
                break
        try:
            await target.get("https://outlook.office.com/", referrer="https://google.com", wait_load=True, timeout=10)
        except asyncio.TimeoutError:
            pass

        # choose an account
        try:
            elem = await target.find_element(By.XPATH, f'//div[@data-test-id]', timeout=10)
        except (asyncio.TimeoutError, NoSuchElementException):
            pass
        else:
            await elem.click()

        # new E-Mail
        elem = await target.find_element(By.XPATH, '//button[@aria-label="Neue E-Mail" or @aria-label="New mail"]',
                                         timeout=15)
        await elem.click()
        await asyncio.sleep(1)

        # to
        elem = await target.find_element(By.XPATH, '//div[@aria-label="An" or @aria-label="To"]', timeout=10)
        await elem.execute_script("obj.textContent = arguments[0]", to)
        await asyncio.sleep(1)

        # Cc
        if cc is not None:
            elem = await target.find_element(By.XPATH, '//div[@aria-label="Cc"]', timeout=10)
            await elem.execute_script("obj.textContent = arguments[0]", cc)
            await asyncio.sleep(1)

        # subject
        elem = await target.find_element(By.XPATH,
                                         '//input[@aria-label="Betreff hinzufügen" or @aria-label="Add a subject"]',
                                         timeout=5)
        await elem.write(betreff)
        await asyncio.sleep(1)

        # write content
        elem = await target.find_element(By.XPATH, '//div[@class="elementToProof"]', timeout=5)
        await elem.write(content)
        await asyncio.sleep(1)

        # send
        elem = await target.find_element(By.XPATH, '//button[@aria-label="Senden" or @aria-label="Send"]', timeout=5)
        await elem.click()
        await asyncio.sleep(10)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("to", help="The destination to send the E-Mail to", type=str)
    parser.add_argument("subject", help="Subject to send the E-Mail with", type=str)
    parser.add_argument("content", help="The content to send", type=str)
    parser.add_argument('--headfull', action='store_true', help="open a window for edge")
    parser.add_argument('--cc', default=None, type=str)
    args = parser.parse_args()
    asyncio.run(write_email(args.to, args.subject, args.content, cc=args.cc, headless=not args.headfull))
    print("E-Mail written")
