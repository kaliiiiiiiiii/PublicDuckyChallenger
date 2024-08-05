from PyInstaller.__main__ import run
from urllib import parse


def build():
    run(pyi_args=["--onefile", "--collect-data", "selenium_driverless", "--collect-data", "numpy", "--collect-data",
                  "scipy", "--nowindowed", "steal.py"])


if __name__ == "__main__":
    build()
