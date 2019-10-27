#Must be run after arduino starts sending
#delay-proof
import os
import sys
from pathlib import Path
import django
from bluetooth import *

currentPath = Path(os.getcwd())
sys.path.append(str(currentPath.parent.parent))
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'webview.settings')
django.setup()
client_socket=BluetoothSocket(RFCOMM)
client_socket.connect(("98:D3:71:FD:7C:19",1))
from monitor.models import Fposition, Sposition

def f_save(distance,angle):
    position = Fposition.objects.create(angle=angle,distance=distance)
    position.save()
    
dp=0;ap=0
flag = False
msg = ''
while True:
    try:
        chunk = client_socket.recv(32).decode("utf-8")
    except:
        continue
    for elem in chunk:
        if elem == '!':
            flag = True
            continue
        if elem == '#':
            msg = msg.strip()
            try:
                d,a = map(float,msg.split())
            except:
                d = dp
                a = ap
            #print(d,a)
            f_save(d,a)
            dp = d
            ap = a
            flag = False
            msg = ''
        if flag:
            msg += elem

    '''
    if msg == b'[':
        continue
    else :  
        msg = msg.replace(b'[',b'') #all sorts of exceptions
        msg = msg.replace(b'\r',b'')
        dic = msg.split(b'\n')
        try:
            print(float(dic[0]),float(dic[1]))
            f_save(float(dic[0]),float(dic[1]))
        except:
            continue
    '''
client_socket.close()
