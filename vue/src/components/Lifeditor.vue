<script setup>
import buda from '../buda.js'
import BudaDate from '../comps/Date.vue'

import { reactive, onMounted } from 'vue'

let life=localStorage.getItem("life"); if(!life) life={
  name:"",
  title:"",
  birth:{ year:1900, month: 1, day: 1 },
  summary:'',
  events:[
    {
      date:{ year:1900, month: 1, day: 1 },
      content: '',
    },
  ],
}; else life=JSON.parse(life);
life = reactive(life);


let PreventSave=false, Pending=false;
function Input(event){
  if(event.target.tagName.toLowerCase()=='textarea')  Buda.dom.setAreaH(event.target, buda.ui.rem);

  //Pending=true; if(PreventSave) return; PreventSave=true;
  localStorage.setItem("life", JSON.stringify(life)); Pending=false;

  // setTimeout(() => { PreventSave=false; if(Pending) Input();  }, 5000);

  //console.log();
}


onMounted(() => {
 // Buda.window_resize();
});

</script>

<template>
  <div class="page_comp_small_padding">
    <div class="field_editor">
      <div class="field_editor_label">标题</div>
      <div class="field_editor_content"><input v-model="life.title" @input="Input" class="field_editor_input_long"/></div>
    </div>
    <div class="field_editor">
      <div class="field_editor_label">出生时间</div>
      <div class="field_editor_content"><BudaDate v-model:year="life.birth.year" v-model:month="life.birth.month" v-model:day="life.birth.day" @input="Input" /></div>      
    </div>  
    <div class="field_editor">
      <div class="field_editor_label">名称</div>
      <div class="field_editor_content"><input v-model="life.name" @input="Input"  class="field_editor_input"/></div>      
    </div>  
    <div class="field_editor_multilines">
      <div class="field_editor_label">综述</div>
      <div class="field_editor_content"><textarea v-model="life.summary" @input="Input" class="field_editor_input_long"></textarea></div>
    </div> 
    <div>
      <div v-for="e in life.events" class="field_editor_verticle">
        <div>
          <div class="field_editor_label">事件</div>
          <BudaDate v-model:year="e.date.year" v-model:month="e.date.month" v-model:day="e.date.day" @input="Input" />
        </div>
        <div class="field_editor_content"><textarea  v-model="e.content"  @input="Input" class="field_editor_input_long"></textarea></div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.company{font-weight: bold;}
.detail_image{ width:100%; border-radius: 0.5rem; margin: 1rem 0 0 0; }
.contacts_c{display: flex;}
.contacts{ display:flex;}
.contact{ margin: 0 1rem 0 0; }
</style>
