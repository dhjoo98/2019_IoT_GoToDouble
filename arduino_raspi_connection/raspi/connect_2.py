#Must be run after arduino starts sending
from bluetooth import *
import requests,json

url = "http://127.0.0.1:5000/sposition"
client_socket=BluetoothSocket(RFCOMM)

client_socket.connect(("98:D3:51:FD:AB:20",1))

i = 0;

while True:
    msg = client_socket.recv(1024)
    try :
        int(msg)
        print("received message :{}".format(msg))
    except:
        continue
print("Finished")
client_socket.close()