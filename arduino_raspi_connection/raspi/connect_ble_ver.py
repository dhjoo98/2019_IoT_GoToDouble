#Must be run after arduino starts sending
from bluetooth import *

client_socket=BluetootSocket(RFCOMM)

client_socket.connect(("98:D3:71:FD:7C:19",1))

i = 0;

while True:
    msg = client_socket.recv(255) #1024
    
    if msg == '[':
        continue
    else :
        i += 1;
        if (i%2) ==1 :    
            msg = msg.replace('[','') #all sorts of exceptions
            msg = msg.replace('\r','')
            dic = msg.split('\n')
            try:
                print([int(dic[0]),int(dic[1])]) #also exception-proof
            except:
                print('error')
                continue
    
print("Finished")
client_socket.close()
