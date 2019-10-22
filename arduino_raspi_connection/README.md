**아두이노-라즈베리파이 간 블루투스 통신 구축하기**

On Windows 7
md written on hackmd.io


---

1. 아두이노-HC06 연결

![](https://i.imgur.com/iN8fjrd.png)


bluetooth_check.ino를 통해 연결확인
시리얼 모니터에서 AT+NAME*namehere* 으로 이름 설정. 예시의 경우 :DOUBLE과 :TRIPLE

send_signal.ino 업로드

---

2. HC06 MAC 주소 찾기. 

윈도우의 '블루투스 장치 추가'에서 정한 이름의 장치 찾기, 오른쪽 클릭, 속성

![](https://i.imgur.com/q3WkXrP.png)

고유 식별자 항목에서 Mac 주소 습득



---

3. 라즈베리파이 세팅 

운영체제 설치된 라즈베리파이가 있다고 가정. 
이더넷으로 PC와 연결, 전원 공급

PC의 제어판에서 네트워크 공유센터, 기존 네트워크의 속성,
공유 창에서 "다른 네트워크 사용자가 이 컴퓨터의....." 체크
홈 네트워킹 연결 - 로컬 영역 연결 선택

![](https://i.imgur.com/KdJ4YQE.png)

(잘 안되도 여러번 체크해제했다가 다시하면 연결된다.)


---

4. 라즈베리 파이 원격 접속 (라파 전원 켤 때 마다 필요)

cmd에 ipconfig 입력, 이더넷 어댑터 로컬 의 IPv4 주소. 

![](https://i.imgur.com/t0KclBO.png)


'Advanced IP Scanner' - 별도 다운로드 필요
IPv4주소의 맨 끝자리만 254로 바꿔 스캔

![](https://i.imgur.com/3tHXxIQ.png)

라즈베리파이의 IP 주소를 구할 수 있다. 

'Putty' - 별도 다운로드 필요 
에서 원격 ssh로 접속 

![](https://i.imgur.com/8l3C04Y.png)

sudo apt-get -y install xrdp  설치

'원격 컴퓨터 연결' - 윈도우 preinstalled 
에 IP 입력하여 원격 접속

![](https://i.imgur.com/IiNv2dE.png)


---

5. 라즈베리파이에서 블루투스 입력 받기

$ sudo apt-get install bluetooth blueman bluez
$ sudo apt-get install python-bluetooth
$ sudo reboot

(4번과정 다시)

$ sudo bluetoothctl
$ scan on
$ agent on
$ pair *HC06의 MAC 주소*
$ default-agent
$ exit

connect.py 파일 실행. 

*Troubleshooting)*
bluetoothctl errors
1. not available -> $scan on 먼저 실행
2. not Authorized -> $agent on 먼저 실행
3. line busy -> ctl에서 모든 블루투스 장치 disconnect

---

두 번째 아두이노를 연결할 경우
새로운 connect_2.py를 작성하고,
$ sudo bluetoothctl 에서
$ trust | disconnect
$ scan on
$ agent on
$ pair *두번째 HC06 주소*
$ default-agent
$ exit

connect_2.py 실행