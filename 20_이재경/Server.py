# simple inquiry example

import bluetooth
import sys
import time
import datetime
import os

bd_addr = "00:21:13:00:e2:b3" #itade address
port = 1



def connect_to_speedgate():
    while 1:
        try:
            new_sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            new_sock.connect((bd_addr, port))
        except Exception as e:
            print("Error occured in connection to speedgate")
            print(e)
            time.sleep(1)
            continue
        else:
            break
    print("Connection successful")
    return new_sock


if not os.path.exists(r"C:\Users\Administrator\Desktop\SpeedGate"):
    os.makedirs(r"C:\Users\Administrator\Desktop\SpeedGate")

b_sock = connect_to_speedgate()

recvList = list();
checkList = list();
cnt = 0
while 1 :
    while cnt < 5:
        try:
            data = b_sock.recv(1)
            checkList.append(int.from_bytes(data, byteorder = 'big' ))
        except:
            print("Connection lost in recving data")
            b_sock = connect_to_speedgate()

        #Check checksum and flush
        if cnt == 4:
            if checkList[0] != checkList[1] ^ checkList[2] ^ checkList[3] ^ checkList[4]:
                data = b_sock.recv(1)
                recvList.clear()
                checkList.clear()
                cnt = 0
                continue
            
        recvList.append(hex(int.from_bytes(data, byteorder = 'big' ))[2:])
        cnt+=1
      
    now = datetime.datetime.now()
   
    try:
        f = open(r"C:\Users\Administrator\Desktop\SpeedGate\Data.txt")
    except FileNotFoundError as e:
        print(e)
        
    while 1:
        dataString = f.readline()
        dataList = dataString.split(",")
        if not dataString:
            permission = 'd'
            break
        if recvList[1] == dataList[1] and recvList[2] == dataList[2] and recvList[3] == dataList[3] and recvList[4] == dataList[4]:
            permission = 'p'
            break;
        
    f.close()

    
    if permission == 'p' and (((now.hour <= 6 and now.hour >= 23 ) and dataList[5] == 's\n') or dataList[5] == 'm\n'):
        print("It's matched")
        try:
            b_sock.send("p")
        except:
            print("Connection lost in sending data")
            b_sock = connect_to_speedgate()
    else:
        try:
            b_sock.send("d")
        except:
            print("Connection lost in sending data")
            b_sock = connect_to_speedgate()
    
    recvList.clear()
    dataList.clear()
    checkList.clear()
    cnt = 0
    time.sleep(1)
    


