<script setup>
import { onMounted , onUnmounted } from 'vue'
import buda from '../buda.js'

const props = defineProps({
  type: String,
  func: Function,
  input_classes:{
    type: Object,
    default(rawProps) {
      return { field_editor_input: true };
    }
  },
  minh: {
    type: Number,
    default: 2
  }
}); 
const value = defineModel();

const {func, type, minh, input_classes}=props; // console.log('minh', minh, value.value);
if(type=='textarea') input_classes.auto_height=true;


let PreventSave=false, SavedValue='';
function NotFunc(event){ return  event.target.value==SavedValue; }
function func2(event){
  if(NotFunc(event)) return; 
  setTimeout(() => { func(); SavedValue=value.value;  event.target.style.color='black'; }, 100);     
}
function Input(event){
  if(NotFunc(event)) return; event.target.style.color='red';
  if(type=='textarea')  Buda.dom.setAreaH(event.target, buda.ui.rem, 0,null, 0, minh);
  if(PreventSave) { return; } PreventSave=true;
  func2(event);
  setTimeout(() => { func2(event); PreventSave=false; }, 5000);
}


onMounted(() => {  
});

onUnmounted(()=>{
   
});


</script>

<template>
    <textarea v-if="type=='textarea'" v-model="value"  @input="Input" @blue="func2" :class="input_classes"></textarea>
    <input v-else v-model="value" @input="Input" @blue="func2" :class="input_classes"/>
</template>

<style scoped>

</style>
