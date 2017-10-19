const express = require('express');
const path = require('path');
const app = express();
const http = require('http').Server(app);
const io = require('socket.io')(http);
const cors = require('cors');
//noble는 우분투에서 설치해야 함. 
const noble = require('noble');

app.use(cors());
app.use(express.static(__dirname + '/dist'));


const diseaseList = require('./diseaseList.js')();
const testingUser = require('./testingUser.js')();
//나이와 병에 따라 maxData를 설정하는 모듈. 
for (let i = 0; i < testingUser.length; i++) {

  for (let j = 0; j < diseaseList.length; j++) {

    if (testingUser[i].disease === diseaseList[j].name) {
      diseaseIndex = diseaseList[j].index;
      break;
    }
  }
  testingUser[i].maxData = 220 - testingUser[i].age - diseaseIndex;
  testingUser[i].timesList = ['00:00', '01:15', '02:30', '03:45', '05:00', '06:15', '07:30', '08:45', '10:00', '11:15', '12:30', '13:45', '15:00', '16:15', '17:30', '18:45', '20:00', '21:15', '22:30', '23:45'];
  testingUser[i].heartsList = [120, 120, 140, 160, 180, 200, 180, 120, 160, 180, 200, 220, 200, 220, 220, 180, 230, 160, 140, 120];
}

//유명환씨의 데이타를 설정하는 모듈. 
const isClassAlert = (sensorValue, maxData)=>{
  
  if(sensorValue < 60 || sensorValue > maxData){
    return "alert";
  }else{
    return "success"; 
  }
}
let sensorValue4 = 100; 
let maxData4 = testingUser[4].maxData; 
let class4 = "success";
//블루투스관련모듈
const PALZZIUUD = ['dfb0'];
noble.on('stateChange', function (state) {

  if (state === 'poweredOn') {
    console.log("블루투스 모듈을 찾습니다.")
    noble.startScanning(PALZZIUUD, false);
  } else {
    noble.stopScanning();
  }
})

noble.on('discover', function (peripheral) {
  // we found a peripheral, stop scanning
  noble.stopScanning(function () {
    peripheral.connect(function (err) {

      if (err) console.log(err);
      console.log('팔찌 모듈과 접속완료!');
      peripheral.discoverServices(PALZZIUUD, function (err, services) {
        services.forEach(function (service) {
          service.discoverCharacteristics([], function (err, characteristics) {

            characteristics.forEach(function (characteristic) {
              /* 팔찌로부터 받는 DATA FORMAT
               *{
               *  "id":정수값,
               *  "name":정수값,
               *  "data":정수값,
               *  "age":정수값,
               *  "disease": 정수값 | 정상 : 0, 부정맥: 1, 천식 : 2, 전날음주 : 3, 흡연 : 4, 당뇨병 5
               *}  
               *테스트는 유명환선생님의 데이타로 한다. 
               */ 
              characteristic.on('read', function (jsonData) {
                console.log("센서값을 정상적으로 수신하였습니다. ")
                console.log(jsonData);
                sensorValue4 = jsonData.data;
                class4 = isClassAlert(sensorValue4, maxData4);  

              })

              //팔찌로 보내주는 진동여부 결정 0이 진동 x, 1이 진동o  
              if(class4 === "alert"){
                const vibrationData = new Buffer(1);
                characteristic.write(vibrationData, false, function (err) {
                  console.log(err)
                })
              } 
            })
          })
        })
      })
    })
  });
})
const getSensor = (sensorValue, maxData) => { 
  const d = new Date();
  const hour = ("0" + d.getHours()).slice(-2);
  const mininute = ("0" + d.getMinutes()).slice(-2);
  const time = hour + ":" + mininute;

  let heartAndClass = {
    "value": sensorValue,
    "class": "success",
    "time": time
  };
  heartAndClass.class = isClassAlert(sensorValue,maxData); 

  return heartAndClass;
}

const makeData = () => {
  let userlist = [];

  for (let i = 0; i < testingUser.length; i++) {
    let diseaseIndex = 0;
    const sensorValue = Math.floor(Math.random() * 160) + 60;
    let heartAndClass; 

    if(i === 4){
      testingUser[i].heart = sensorValue4; 
      testingUser[i].class = class4; 
      heartAndClass = getSensor(sensorValue4, maxData4); 
    }else{
      heartAndClass = getSensor(sensorValue, testingUser[i].maxData); 
    }

    testingUser[i].timesList.shift();
    testingUser[i].timesList.push(heartAndClass.time);
    testingUser[i].heartsList.shift();
    testingUser[i].heartsList.push(heartAndClass.value);

    const testObj = {
      "id": i + 1,
      "name": testingUser[i].name,
      "heart": heartAndClass.value,
      "age": testingUser[i].age,
      "class": heartAndClass.class,
      "disease": testingUser[i].disease,
      "chartData": {
        "times": testingUser[i].timesList,
        "heartData": testingUser[i].heartsList,
        "minData": 60,
        "maxData": testingUser[i].maxData,
      }
    };
    userlist.push(testObj);
  }
  return userlist;
}

io.on('connection', function (socket) {
  //userlist를 만드는 모듈  
  setInterval(() => {
    const userlist = makeData();
    socket.emit('sendUserlist', userlist);
  }, 1000 * 5);

})
http.listen(52273, () => {
  console.log("52273번 포트에서 시작됩니다.")
})
