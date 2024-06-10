<script setup>
import buda from '../buda.js'
import { onMounted, ref } from 'vue'

let name=ref("aa");
let list=['div1', 'div2', 'div3'];

function update(){
  name.value="bb";
  list[1]="div4";
}

buda.page.name='测试页面';

onMounted(() => {
  
});
</script>
<template>
  <div>
    <button @click="update">update</button>

    <div class="about">
      {{ name }}
    </div>

    <div>
      <div v-for="div of list" :id="div+'div'">{{ div }} <input :id="div+'input'"/></div>
    </div>
  </div>
</template>

<style>

</style>
