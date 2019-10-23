from bluetooth import *

client_socket=BluetoothSocket(RFCOMM)

client_socket.connect(("98:D3:51:FD:AB:20",1)) #address of 2nd HC06

while True:
    msg = client_socket.recv(1024)
    print("received message :{}".format(msg))
    
print("Finished")
client_socket.close()