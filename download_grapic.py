import os
import urllib.request

BASE_URL = "https://raw.githubusercontent.com/ucacaxm/grapic/master"
FILES = ["src/Grapic.cpp", "src/Grapic.h", 'data/ttf/Roboto-Regular.ttf']

for fname in FILES:
    print(f"Downloading '{fname}'...")

    os.makedirs("/".join(fname.split("/")[:-1]), exist_ok=True)

    url = "/".join([BASE_URL, fname])
    res = urllib.request.urlopen(url)

    if (res.status == 200):
        open(fname, "wb").write(res.read())
    else:
        print(f"There was an error downloading {fname} (error {res.status})")

print("Successfully downloaded Grapic.")