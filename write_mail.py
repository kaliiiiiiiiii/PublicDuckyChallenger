import asyncio

from edge_email import write_email
import argparse


async def main(to: str, cc: str = None, headfull: bool = False) -> None:
    subject = "Form"
    content = \
        """
Good afternoon

Please fill in the for form at https://is.gd/tuioj

With kind regards'
Steve
"""

    await write_email(to, subject, content, cc=cc, headless=not headfull)
    print("E-Mail written")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("to", help="The destination to send the E-Mail to", type=str)
    parser.add_argument('--headfull', action='store_true', help="open a window for edge")
    parser.add_argument('--cc', default=None, type=str)
    args = parser.parse_args()
    asyncio.run(main(args.to, cc=args.cc, headfull=args.headfull))
