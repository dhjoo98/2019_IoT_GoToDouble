const express    = require('express');
const path       = require('path');
const app        = express();
const http       = require('http').Server(app);
const io         = require('socket.io')(http);
const cors       = require('cors');
const favicon    = require('serve-favicon');
const request    = require('request');
const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
app.use(cors());
app.use(express.static(__dirname + '/dist'));
app.use(favicon(path.join(__dirname, 'favicon.ico')))


const diseaseList = require('./diseaseList.js')();
const testingUser = require('./testingUser.js')();

//기본데이타 설정 : 나이와 병에 따라 maxData를 설정하여 testingUser이란 json에 등록시킨다. 
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
//센서로부터 받아온 데이타를 바탕으로 그 부분의 maxData와 sensor를 설정하는 모듈 
//초기 데이타 설정
//disease는 병명을 나타내는 코드. 0 : 정상, 1: 부정맥, 2: 천식, 4: 전날음주, 8: 흡연, 16: 당뇨병
let sensorUserList = [{
  "id": 0,
  "name": "주홍철",
  "data": 30,
  "age": 25,
  "disease": 1,
}, {
  "id": 4,
  "name": "유명환",
  "data": 100,
  "age": 30,
  "disease": 1
}];

let count = 1;
let beforecount = 0;
app.post('/getData', (req, res) => {
  console.log('Post요청이 들어옵니다.')   
  let stringBody = req.body.toString(); 
  stringBody.replace("'", "");
  stringBody     = "[" + stringBody + "]"; 
  const jsonList = JSON.parse(stringBody);
  console.log(jsonList); 
  sensorUserList = jsonList;
  count = count + 1; 
  res.sendStatus(200);
}); 



const isClassAlert = (sensorValue, maxData) => {

  if (sensorValue < 60 || sensorValue > maxData) {
    return "alert";
  } else {
    return "success";
  }
}

const makeOnePerson = (sensorValue, maxData) => {  
  const d        = new Date();
  const hour     = ("0" + d.getMinutes()).slice(-2);
  const mininute = ("0" + d.getSeconds()).slice(-2);
  const time     = hour + ":" + mininute;

  let heartAndClass = {
    "value": sensorValue,
    "class": "success",
    "time": time
  };
  heartAndClass.class = isClassAlert(sensorValue, maxData);
  return heartAndClass;
}

const getSensor = (maxData) => {
  // const sensorValue = Math.floor(Math.random() * 160) + 60;
  const sensorValue   = 100;
  const d             = new Date();
  const hour          = ("0" + d.getMinutes()).slice(-2);
  const mininute      = ("0" + d.getSeconds()).slice(-2);
  const time          = hour + ":" + mininute;

  let heartAndClass = {
    "value": sensorValue,
    "class": "success",
    "time": time
  };
  heartAndClass.class = isClassAlert(sensorValue, maxData);

  return heartAndClass;
}

//disease는 병명을 나타내는 코드. 0 : 정상, 1: 고혈압, 2: 천식, 4: 전날음주, 8: 흡연, 16: 당뇨병
const makeMaxDataAndDisease = (diseaseIndex, age) => {
  let resultString = "";
  let resultDiseaseIndex = 0; 
  if (diseaseIndex == 0) {
    resultString += "정상";
  }

  if (diseaseIndex & 1) {
    resultString += "고혈압";
    resultDiseaseIndex += 10;
  }

  if (diseaseIndex & 2) {
    resultString += "천식";
    resultDiseaseIndex += 20;
  }

  if (diseaseIndex & 4) {
    resultString += "전날음주";
    resultDiseaseIndex += 20;
  }

  if (diseaseIndex & 8) {
    resultString += "흡연";
    resultDiseaseIndex += 30;
  }

  if (diseaseIndex & 16) {
    resultString += "당뇨병";
    resultDiseaseIndex += 15;
  } 
  const resultObj = {
    "disease": resultString,
    "maxData": 220 - age - resultDiseaseIndex
  }
  return resultObj;
}

const makeData = () => {
  let sensorIdList = []; 
  
  for (let i = 0; i < sensorUserList.length; i++) {
    sensorIdList.push(sensorUserList[i].id);
    const sensorDiseaseMax = makeMaxDataAndDisease(sensorUserList[i].disease,sensorUserList[i].age);
    sensorUserList[i].diseaseStr = sensorDiseaseMax.disease;
    sensorUserList[i].maxDataResult = sensorDiseaseMax.maxData;
  } 
  let userlist = [];

  for (let i = 0; i < testingUser.length; i++) {
    let heartAndClass;

    if (sensorIdList.includes(testingUser[i].id)) {
      
      for(let j = 0; j < sensorUserList.length; j++){
        
        if(testingUser[i].id === sensorUserList[j].id){ 
          testingUser[i].id      = sensorUserList[j].id;
          testingUser[i].name    = sensorUserList[j].name;
          testingUser[i].age     = sensorUserList[j].age; 
          testingUser[i].disease = sensorUserList[j].diseaseStr;
          heartAndClass          = makeOnePerson(sensorUserList[j].data, sensorUserList[j].maxDataResult);
        }
      } 
    } else {
      heartAndClass = getSensor(testingUser[i].maxData);
    }

    testingUser[i].timesList.shift();
    testingUser[i].timesList.push(heartAndClass.time);
    testingUser[i].heartsList.shift();
    testingUser[i].heartsList.push(heartAndClass.value);

    const testObj = {
      "id": testingUser[i].id,
      "name": testingUser[i].name,
      "heart": heartAndClass.value,
      "class": heartAndClass.class,
      "age": testingUser[i].age,
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
  if(count > beforecount){
    const userlist = makeData();
    socket.emit('sendUserlist', userlist);
    
  }
  // setInterval(() => {
  //   const userlist = makeData();
  //   socket.emit('sendUserlist', userlist);
  // }, 1000 * 1);

})
http.listen(52273, () => {
  console.log("52273번 포트에서 시작됩니다.")
})
