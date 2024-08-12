from PyInstaller.__main__ import run
import argparse


def build(file: str = "steal.py"):
    run(pyi_args=["--onefile", "--collect-data", "selenium_driverless", "--collect-data", "numpy", "--collect-data",
                  "scipy", "--nowindowed", file])


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help="python file to build for", type=str, default="steal.py")
    args = parser.parse_args()
    build(file=args.file)
