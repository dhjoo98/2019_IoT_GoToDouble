client_socket=BluetoothSocket(RFCOMM)

client_socket.connect(("98:D3:71:FD:7C:19",1))

while True:
    msg = client_socket.recv(1024)
    try :
        int(msg)
        print("received message :{}".format(msg))
    except:
        continue
print("Finished")
client_socket.close()
