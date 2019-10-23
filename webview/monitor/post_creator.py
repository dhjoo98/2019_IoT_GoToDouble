import requests,json
furl = "http://127.0.0.1:5000/fposition"
surl = "http://127.0.0.1:5000/sposition"
angle = 0.0
distance = 0.0
while True:
    dummy_fdata = {"angle": str(angle), "distance":str(distance)}
    dummy_sdata = {"angle": str(angle+1.2), "distance":str(distance+0.2)}
    fpost = requests.post(url=furl,data=dummy_fdata)
    spost = requests.post(url=surl,data=dummy_sdata)
    angle = 0 if angle>180 else angle+1
    distance = 0 if distance>160 else distance+1
