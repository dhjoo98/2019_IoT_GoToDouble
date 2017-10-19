<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<script>
  export default {
    name: 'app'
  }

</script>

<style>  
  body{
    background: #f9f9f9;
    margin: 0 auto;
  }
  @font-face {
    font-family: 'Kakao'; 
    src: url(./assets/fonts/KakaoOTF-aLt.woff2) format('woff2')
  } 

  #app {
    font-family: 'Kakao'; 
    text-align: center;
  }

</style>
