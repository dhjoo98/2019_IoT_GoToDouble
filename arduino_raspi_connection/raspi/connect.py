from bluetooth import *
import requests,json

url = "http://127.0.0.1:5000/fposition"
client_socket=BluetoothSocket(RFCOMM)

client_socket.connect(("98:D3:71:FD:7C:19",1)) #MAC of first HC06

while True:
    distance,angle = map(float,client_socket.recv(1024).split())
    data = {"angle": str(angle), "distance": str(distance)}
    requests.post(url=url,data=data)

client_socket.close()
