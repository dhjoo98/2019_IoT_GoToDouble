<h1> Brain storming </h1>  
일단 API 서버를 구축하기 위하여 생각을 정리하여 보았습니다.<br><br>

![first](./uploads/brainstorming.jpg)

<h1> Block diagram </h1>  
생각을 정리한 후 Block diagram을 그려서 개발 목적 및 방향을 구체화 하였습니다.<br><br>

![second](./uploads/diagram.PNG)  

<h1> 개발 환경 </h1>
<h3> Python 3.6.8 </h3>
<h3> Django 2.2.6 (Virtual environment) </h3>
<h3> Django rest framework 3.10.3 (Virtual environment) </h3>

<h1> API Server 구조 </h1>

![third](./uploads/architecture.PNG)

<h1> Test 결과 </h1>
Test 코드는 python을 이용하여 작성하였습니다.<br>
<h3> Test code </h3>

```python
import requests,json

furl = "http://127.0.0.1:8000/fposition"
surl = "http://127.0.0.1:8000/sposition"
dummy_fdata = {"angle": "90.0", "distance":"125.0"}
dummy_sdata = {"angle": "80.0", "distance":"124.0"}
fpost = requests.post(url=furl,data=dummy_fdata)
spost = requests.post(url=surl,data=dummy_sdata)
print("POST response data")
print(fpost.json())
print(spost.json())
fget = requests.get(url=furl)
sget = requests.get(url=surl)
print("GET response data")
print(fget.json())
print(sget.json())
```
![fourth](./uploads/test.PNG)  

<h1> Trouble Shooting </h1>
<h2> Imporve performance </h2>
기능 구현 후 benchmark를 수행하였습니다. 나쁘지 않은 성능이지만 더 개선할 수 있다고 생각하여 고민을 해봤습니다.<br>
<img src="https://user-images.githubusercontent.com/18005162/67508214-844ddc80-f6cb-11e9-8ccf-07a95fd7e9b2.PNG"></img>

<h3> Top-Down View </h3>
다음과 같은 순서로 개선 방향을 잡았습니다.<br>

  1.  <strong>구조 개선</strong>: 기존 django-restful api의 통신 구조가 적합하지 않다고 생각이 들었습니다.<br>
    적합한 구조를 찾다가 django-channels를 이용해 보자는 생각이 들었습니다.<br>
    하지만 release된 지 얼마 되지 않아서 그런지 python 버전과 호환성 문제가 발생하여 결국 차선책을 찾게 되었습니다.<br>
    추후 관련 사항을 정리하여 django-channels issue에 report할 생각입니다.<br>
  
  2.  <strong>통신 시스템 개선</strong>: 불필요한 django-restful api의 통신 횟수를 줄여 보자는 생각이 들었습니다.<br>
    raspberry pi에서 django orm에 직접 접근할 수 있다면 POST method에 의한 django api server 부하를 줄일 수 있습니다.<br>
    django-management를 이용한 방법과 python script상에서 직접 django path를 설정하는 방법 두 가지를 검증해 보았습니다.<br>
    전자는 manage.py를 사용하기에 후자가 더 안정성이 있다고 판단하였고, 이를 실제 사용하였습니다.<br> 
  
<h2> Stable feature </h2>
기능 완성 후 여러 차례 테스트를 해보던 도중 가끔씩 잘못 된 데이터가 입력으로 들어오는 것을 확인하였습니다.<br>

<img src="https://user-images.githubusercontent.com/18005162/67508224-8879fa00-f6cb-11e9-9cd8-5209d5b147e2.PNG"></img>

이를 해결하기 위해 arduino-raspberry pi간의 bluetooth통신, raspberry-django간의 data 접근을 각각 검증해 보았습니다.<br>

  1.  <strong>arduino-raspberry pi간 bluetooth 통신</strong>: arduino의 serial monitor에 출력해 본 send data에는 문제가 없었습니다.<br>
    하지만 raspberry pi상에서 receive한 data를 출력해 본 결과 간혹 이상한 데이터가 나온다는 것을 확인하였습니다.<br>
    bluetooth socket에서 send한 data를 읽어올 때 데이터의 시작과 끝을 알 수 없기에 발생하는 문제라고 유추하였습니다.<br>
    시작과 끝을 구별해 주는 special character '!', '#'를 이용하여 해결하였습니다.<br>
    
   <img src="https://user-images.githubusercontent.com/18005162/67508225-8adc5400-f6cb-11e9-8597-122b9334b9ed.PNG"></img>
    
  2.  <strong>raspberry pi-django간 data 접근</strong>: django database에 lock이 걸리기 때문에 문제가 없습니다.<br>
    1번의 문제를 해결하니 해당 증상이 사라져 이 부분에는 문제가 없다고 판단하였습니다.<br>

    
  
