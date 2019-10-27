#Must be run after arduino starts sending
#delay-proof
from bluetooth import *

client_socket=BluetoothSocket(RFCOMM)

client_socket.connect(("98:D3:91:FD:83:4B",1))

i = 0;
'''
msg = 'AT'
client_socket.send(msg)
print(msg)
'''
while True:
    msg = client_socket.recv(255) #1024
    #print(msg)
    if msg == '[':
        continue
    else :  
        msg = msg.replace('[','') #all sorts of exceptions
        msg = msg.replace('\r','')
        dic = msg.split('\n')
        try:
            print(float(dic[0]),float(dic[1])) #also exception-proof
        except:
            continue

print("Finished")
client_socket.close()