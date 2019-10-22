from bluetooth import *

client_socket=BluetootSocket(RFCOMM)

client_socket.connect(("98:D3:71:FD:7C:19",1)) #MAC of first HC06

while True:
    msg = client_socket.recv(1024)
    print("received message :{}".format(msg))
    
print("Finished")
client_socket.close()