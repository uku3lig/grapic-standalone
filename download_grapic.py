import os
import urllib.request

BASE_URL = "https://raw.githubusercontent.com/ucacaxm/grapic/master/src"
FILES = ["Grapic.cpp", "Grapic.h"]

os.makedirs("src", exist_ok=True)
for fname in FILES:
    print(f"Downloading '{fname}'...")
    url = "/".join([BASE_URL, fname])

    res = urllib.request.urlopen(url)
    if (res.status == 200):
        open(f'src/{fname}', "wb").write(res.read())
    else:
        print(f"There was an error downloading {fname} (error {res.status})")

print("Successfully downloaded Grapic.")