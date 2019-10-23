from bluetooth import *
import requests,json

url = "http://127.0.0.1:5000/sposition"
client_socket=BluetoothSocket(RFCOMM)

client_socket.connect(("98:D3:51:FD:AB:20",1)) #address of 2nd HC06

while True:
<<<<<<< HEAD
    distance,angle = map(float,client_socket.recv(1024).split())
    data = {"angle": str(angle), "distance": str(distance)}
    requests.post(url=url,data=data)

client_socket.close()
=======
    msg = client_socket.recv(1024)
    try :
        int(msg)
        print("received message :{}".format(msg))
    except:
        continue
print("Finished")
client_socket.close()
>>>>>>> Fix to both connect.py s
