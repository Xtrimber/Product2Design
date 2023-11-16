<template>
  <div class="home">
<div class="cabecera">
  <h1>ejemplo</h1>
</div>
<div class="Medio">
  <h1>Gofly</h1>
</div>
<div class="Imagenes">
  <h1>Hire Seadoned Developpers In Your Time Zone.</h1>
  <img id="imgi" src="../assets/descarga.jpg" width="500" heigth="500">
  <img id="imgd" src="../assets/dsa.jpg"  width="250" heigth="500">
</div>
</div>
</template>

<script>
// @ is an alias to /src
import HelloWorld from '@/components/HelloWorld.vue'

export default {
  name: 'HomeView',
  components: {
    HelloWorld
  }
}
</script>



<style>
  .cabecera{
    width: 100%;
    background-color: greenyellow;
  }
  .Imagenes{
    width: 100%;
    image-resolution: 50%;
    
  }
  .Medio{
    background-color: grey;
  }
  
</style>
