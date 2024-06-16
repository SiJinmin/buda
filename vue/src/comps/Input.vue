<script setup>
import { ref, onMounted } from 'vue'
import buda from '../buda.js'

const props = defineProps(['type','func','input_classes','minh','input_id']); 
const value = defineModel();
const {func, type, minh, input_id}=props;


let PreventSave=false, SavedValue='';
function NotFunc(){ return  value.value==SavedValue; }
function func2(){
  if(NotFunc()) return; func(); SavedValue=value.value;
}
function Input(event){
  if(NotFunc()) return;
  if(type=='textarea')  Buda.dom.setAreaH(event.target, buda.ui.rem, 0,null, 0, minh);
  if(PreventSave) { return; } PreventSave=true;
  func2();
  setTimeout(() => { func2(); PreventSave=false; }, 5000);
}


onMounted(() => {  
  if(type=='textarea') { 
    let textarea=document.getElementById(input_id); // console.log('textarea', input_id, textarea);  
    Buda.dom.setAreaH(textarea, buda.ui.rem, 0,null, 0, minh); 
  }
});

</script>

<template>
    <textarea :id="input_id" v-if="type=='textarea'" v-model="value"  @input="Input" @blue="func2" :class="input_classes"></textarea>
    <input v-else v-model="value" @input="Input" @blue="func2" :class="input_classes"/>
</template>

<style scoped>

</style>
