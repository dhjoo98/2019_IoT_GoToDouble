import serial
import time
import requests
import json
import sys
import sqlite3

def alertToUnit(ser):
	ser.write(b"!")

''' 병명에 따른 Flag값
NORMAL 		 = 0
HYPERTENSION = 1<<0
ASTHMA 		 = 1<<1
DRINKED 	 = 1<<2
SMOKER 		 = 1<<3
DIABETES 	 = 1<<4
'''
disabilDict  = {"HYPERTENSION":(0,10),"ASTHMA":(1,20),"DRINKED":(2,20),"SMOKER":(3,30),"DIABETES":(4,15)}

# 블루투스 시리얼 통신 연결. COM포트를 이용한다.
serialDict = dict()
comList = ["COM5"]
while len(serialDict)<len(comList):
	for com in comList:
		if com in serialDict.keys(): pass
		try:
			ser = serial.Serial(com,9600,timeout=0,parity=serial.PARITY_EVEN,rtscts=1)
			print(ser, "is CONNECTED")
			serialDict[com] = ser
			serial
		except Exception as e:
			print(e)

# DB 연결. Sqlite3를 이용한다.
con = sqlite3.connect("unitInformation.db")
cur = con.cursor()
print("sqlite3 is CONNECTED")

reqUrl = "http://10.53.128.177:52273/getData"
sendList = list()
idDict = dict()
readByteSize = 50
idCount = 0

while True:
	time.sleep(1)
	for ser in serialDict.values():
		s = ser.read(readByteSize)
		if len(s)!=0:
			try:
				# 블루투스에 적힌 값을 조금만 가져오는 경우가 있으므로 잠시 대기했다가 추가로 한번더 읽는다.
				time.sleep(0.2)
				s += ser.read(readByteSize)

				# 블루투스로부터 읽어온값은 Byte이므로 다룰 수 있는 JSON으로 utf-8인코딩하여 만든다.
				# 만약 값이 JSON형식에 맞지 않으면 건너뛴다.
				try:
					s = json.loads(s, encoding='utf-8')
				except Exception as e:
					print(e)
					print("MAYBE DATA is NOT JSON format...")
					continue
				
				# 만든 뒤에는 id값을 가져와 DB로부터 서버에 보낼 정보들을 가져오고, 연산작업이 필요하면 해준다. 
				# 연산작업 후 위험하다 판단되면 연결된 Unit에게 위험알림을 해준다. 
				# 이 때 id가 루틴중 이미 추가된 것이면 DB로부터 가져오는것을 생략하고 data만 업데이트 해준다.
				s = dict(s)
				print(idDict)
				unitID = s["id"]
				if unitID in idDict.keys():
					tmpData = s["data"]
					s = idDict[unitID]["unitData"]
					s["data"] = tmpData
				else:
					s["id"] = idCount
					idCount+= 1
					query   = "SELECT name, age FROM HS_BASEINFO WHERE id=" + unitID
					cur.execute(query)
					s["name"], s["age"] = cur.fetchone()
					s["disease"] = 0
					reduceMaxHeartRate = 0
					for da_name,da_value in disabilDict.items():
						query = "SELECT " + da_name + " FROM HS_DISABILITY WHERE id=" + unitID
						cur.execute(query)
						rst = cur.fetchone()
						s["disease"]+=rst[0]<<da_value[0]
						reduceMaxHeartRate+=rst[0]*da_value[1]
					maxHeartRate = 220-s["age"]-reduceMaxHeartRate
					idDict[unitID] = dict()
					idDict[unitID]["unitData"] = s
					idDict[unitID]["maxHeartRate"] = maxHeartRate
				if idDict[unitID]["maxHeartRate"] < s["data"] or s["data"] < 60:
					alertToUnit(ser)
				
				# 서버로 보낼때는 JSON값을 dump시켜 보내야 하므로 dump한 것을 리스트에 추가해준다.
				s = json.dumps(s, ensure_ascii=False)
				sendList.append(s)
				

				# 추가 해준 뒤 보낼 리스트의 크기가 연결된 시리얼의 수와 같아지면 서버에 보내준다. 보낼때는 리스트를 다시 dump시키고 dump된 값을 utf-8로 인코딩하여 서버에서 한글이름을 인식할 수 있도록 해준다.
				if len(sendList)==len(serialDict):
					sendData = json.dumps(sendList, ensure_ascii=False).encode('utf-8')
					headers = {'Content-Type':'application/json', 'Accept':'application/json'}
					res = requests.post(reqUrl, data=sendData, headers=headers)
					print(res)
					
					# 다 보내졌으면 보낼 리스트를 초기화 해준다.
					sendList=[]
				

			except Exception as e: 
				print(e)
				sys.exit(-1)
