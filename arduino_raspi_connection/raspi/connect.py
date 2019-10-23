
i = 0;

while True:
    msg = client_socket.recv(255) #1024

    if msg == '[':
        continue
    else :    
        msg = msg.replace('[','') #all sorts of exceptions
        msg = msg.replace('\r','')
        dic = msg.split('\n')
        try:
            print([int(dic[0]),int(dic[1])]) #also exception-proof
        except:
            #print('error')
            continue

print("Finished")
client_socket.close()
