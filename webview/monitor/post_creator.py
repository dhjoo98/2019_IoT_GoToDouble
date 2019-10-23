import requests,json
furl = "http://127.0.0.1:8000/fposition"
surl = "http://127.0.0.1:8000/sposition"
while True:
    dummy_fdata = {"angle": "90.0", "distance":"125.0"}
    dummy_sdata = {"angle": "80.0", "distance":"124.0"}
    fpost = requests.post(url=furl,data=dummy_fdata)
    spost = requests.post(url=surl,data=dummy_sdata)
