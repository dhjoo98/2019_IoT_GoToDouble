import os
import sys
from pathlib import Path
import django
import datetime
import requests,json


currentPath = Path(os.getcwd())
sys.path.append(str(currentPath.parent.parent))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'webview.settings')
django.setup()
from monitor.models import Fposition, Sposition

furl = "http://127.0.0.1:5000/fposition"
surl = "http://127.0.0.1:5000/sposition"

data = Fposition.objects.all().delete()
dt = datetime.datetime.now()
timer = 0

while True:
    for i in range(30, 150, 2):
        distance = 39
        if i >= 30 and i < 60:
            distance = 20
        elif i >= 60 and i < 90:
            distance = 39
        elif i >= 90 and i < 120:
            distance = 30
        elif i >= 120 and i < 150:
            distance = 39
        angle = i
        dummy_fdata = {"angle": str(angle), "distance":str(distance)}
        dummy_sdata = {"angle": str(angle+1.2), "distance":str(distance+0.2)}
        fpost = requests.post(url=furl,data=dummy_fdata)
        spost = requests.post(url=surl,data=dummy_sdata)

    for i in range(150, 30, -2):
        if i >= 30 and i < 60:
            distance = 20
        elif i >= 60 and i < 90:
            distance = 30
        elif i >= 90 and i < 120:
            distance = 30
        elif i >= 120 and i < 150:
            distance = 39
        angle = i
        dummy_fdata = {"angle": str(angle), "distance":str(distance)}
        dummy_sdata = {"angle": str(angle+1.2), "distance":str(distance+0.2)}
        fpost = requests.post(url=furl,data=dummy_fdata)
        spost = requests.post(url=surl,data=dummy_sdata)

    for i in range(30, 150, 2):
        if i >= 30 and i < 60:
            distance = 20
        elif i >= 60 and i < 90:
            distance = 20
        elif i >= 90 and i < 120:
            distance = 30
        elif i >= 120 and i < 150:
            distance = 39
        angle = i
        dummy_fdata = {"angle": str(angle), "distance":str(distance)}
        dummy_sdata = {"angle": str(angle+1.2), "distance":str(distance+0.2)}
        fpost = requests.post(url=furl,data=dummy_fdata)
        spost = requests.post(url=surl,data=dummy_sdata)
