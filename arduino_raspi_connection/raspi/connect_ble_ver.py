from bluepy import btle
import struct

class MyDelegate(btle.DefaultDelegate):
    def __init__(self,params):
        btle.DefaultDelegate.__init__(self)

    def handleNotification(self,cHandle,data):
        print("handling notification...")
##        print(self)
##        print(cHandle)
        print(struct.unpack("b",data))

p = btle.Peripheral('00:15:87:00:4e:d4')
p.setDelegate(MyDelegate(0))

while True:
    if p.waitForNotifications(1.0):
        continue
    print("waiting...")
    
#source https://makersportal.com/blog/2018/3/25/arduino-internet-of-things-part-4-connecting-bluetooth-nodes-to-the-raspberry-pi-using-pythons-bluepy-library

