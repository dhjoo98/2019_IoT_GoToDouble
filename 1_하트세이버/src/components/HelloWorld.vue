<template>
  <div class="hello">
    <nav>
      <h1>
        <span class="icon heart">
        </span>
        <span>
          하트세이버
        </span>
        <span class="icon setting">
        </span>
      </h1>
    </nav>
    <div class="userListWrap">
      <div class="userWrap" v-for="(user, index ) in userlist" :key="user.num" :class="user.class">
        <div class="userInfo">
          <span>{{user.id}}번 {{user.name}} ( {{user.age}} ) {{user.disease}}</span>
        </div>

        <div class="userHeart">
          <span class="num">{{user.heart}}</span>
          <span class="unit">bpm</span>
        </div>

        <div>
          <chartComponent :chartData="user.chartData"></chartComponent>
        </div>
      </div>
    </div>
    <audio :src="alertMusic"></audio>
  </div>
</template>

<script>
  import chartComponent from './chartComponent';

  import io from '../../lib/socket.io';
  const alertMusic = require('../assets/alert.mp3');
  export default {
    name: 'HelloWorld',
    data() {
      return {
        userlist: {},
        socketServer: null,
        alertMusic: alertMusic
      }
    },
    mounted() {
      this.connect();
    },
    methods: {
      connect() {
        this.socketServer = io.connect('http://127.0.0.1:52273');
        const audio = document.getElementsByTagName("audio")[0];
        //socket으로 userlist를 받는다.한명이라도 위험한 사람이 있다면 오디오가 플레이된다.
        //userlist 에는 아래에 해당하는 객체가 모여있다. 이 데이타를 통해 실시간값, 차트데이타가 생성된다.
        // const testObj = {
        //   "id": testingUser[i].id,
        //   "name": testingUser[i].name,
        //   "heart": heartAndClass.value,
        //   "class": heartAndClass.class,
        //   "age": testingUser[i].age,
        //   "disease": testingUser[i].disease,
        //   "chartData": {
        //     "times": testingUser[i].timesList,
        //     "heartData": testingUser[i].heartsList,
        //     "minData": 60,
        //     "maxData": testingUser[i].maxData,
        //   }
        // };
        this.socketServer.on('sendUserlist', (userlist) => {
          this.userlist = userlist;
          let isPlay = false;
          for (let i = 0; i < this.userlist.length; i++) {
            if (this.userlist[i].class === "alert") {
              isPlay = true;
              break;
            }
          }
          if (isPlay) {
            audio.play();
          } else {
            audio.pause();
          }

        })
      }
    },
    components: {
      chartComponent
    }
  }

</script>
 
<style>
  .userListWrap {
    display: flex;
    flex-wrap: wrap;
    justify-content: space-around;
  }

  .userListWrap>div {
    margin-bottom: 10px;
  }

  .userInfo {
    background-color: #c9e1df;
    padding: 10px;
    font-size: 24px;
  }

  .userHeart {
    top: 26px;
    position: relative;
  }

  .userHeart>.num {
    font-size: 50px;
    font-weight: bold;
  }

  .userWrap {
    display: inline-block;
    border: 1px solid #79b4b0;
    background: #fff;
  }

  .alert {
    background: #c92325 !important;
    animation: .8s infinite beatHeart;
  }

  @keyframes beatHeart {
    0% {
      transform: scale(1);
    }
    25% {
      transform: scale(1.05);
    }
    40% {
      transform: scale(1);
    }
    60% {
      transform: scale(1.05);
    }
    100% {
      transform: scale(1);
    }
  }

  .icon {
    width: 50px;
    height: 50px;
  }

  nav {
    position: relative;
    width: 1000px;
    margin: 0 auto;
  }

  nav span {
    vertical-align: middle;
    display: inline-block;
    position: relative;
  }

  .heart {
    background: url('../assets/heart.png');
  }

  .setting {
    position: absolute;
    top: 0;
    right: 0;
    background: url('../assets/setting.png');
  }

  h1 {
    font-size: 40px;
    margin-bottom: 5px;
    margin-top: 5px;
  }

  td .num {
    font-size: 50px;
    font-weight: bold;
  }

  td .unit {
    font-size: 14px;
  }

</style>
