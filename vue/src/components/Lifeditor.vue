<script setup>
import buda from '../buda.js'
import BudaDate from '../comps/Date.vue'
import BudaInput from '../comps/Input.vue'

import { reactive, onMounted } from 'vue'

function AEvent(year, month, day){
  return {
    date:{ year, month, day },
    content: '',
  };
}

let life=localStorage.getItem("life"); if(!life) life={
  name:"",
  title:"",
  birth:{ year:1900, month: 1, day: 1 },
  summary:'',
  events:[
    AEvent(1900,1,1),
  ],
}; else life=JSON.parse(life);
life = reactive(life);

function LastEvent(){ return life.events[life.events.length-1]; }
function Age(e){ return e.date.year-life.birth.year;  }


let BirthUpdateFirst=true;
function save(){
  localStorage.setItem("life", JSON.stringify(life));
  console.log('saved', new Date());

  let e=life.events[0].date, b=life.birth; let {year, month, day}=e; 
  BirthUpdateFirst = ((year==1900 && month==1 && day==1) || (year==b.year && month==b.month && day==b.day));
}
function save_birth(){ 
  if(BirthUpdateFirst){ let e=life.events[0].date, b=life.birth; e.year=b.year; e.month=b.month; e.day=b.day; }
  save(); 
}

function AddEvent(nexte, nextei){
  let e=nexte; if(!nexte) { e=LastEvent(); nextei=life.events.length; } let d = e.date ;
  life.events.splice(nextei, 0, AEvent(d.year, d.month, d.day));
  save();
}


onMounted(() => {
 // Buda.window_resize();
    let textareas=document.getElementsByClassName('auto_height');  
    for(let textarea of textareas)
    { Buda.dom.setAreaH(textarea, buda.ui.rem, 0,null, 0, 2);  }   
});

</script>

<template>
  <div class="page_comp_small_padding">
    <div class="field_editor">
      <div class="field_editor_label">标题</div>
      <div class="field_editor_content"><BudaInput v-model="life.title" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" :maxh="3"/></div>
    </div>
    <div class="field_editor">
      <div class="field_editor_label">出生时间</div>
      <div class="field_editor_content"><BudaDate v-model:year="life.birth.year" v-model:month="life.birth.month" v-model:day="life.birth.day" :func="save_birth" /></div>      
    </div>  
    <div class="field_editor">
      <div class="field_editor_label">名称</div>
      <div class="field_editor_content"><BudaInput v-model="life.name" :func="save"/></div>      
    </div>  
    <div class="field_editor_multilines">
      <div class="field_editor_label">综述</div>
      <div class="field_editor_content"><BudaInput v-model="life.summary" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" /></div>
    </div> 
    <div>事件</div>
    <div>
      <div v-for="(e, ei) in life.events" class="field_editor_verticle">
        <div>
          <img src="@/assets/add.svg" @click="AddEvent(e, ei)" class="add_img">
        </div>
        <div>
          <div class="event_head">
            <BudaDate v-model:year="e.date.year" v-model:month="e.date.month" v-model:day="e.date.day" :func="save" />
            <div class="event_age">{{ Age(e) }}岁</div>
          </div>
          <div class="field_editor_content"><BudaInput v-model="e.content" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save"/></div>
        </div>
      </div>
      <div>
        <img src="@/assets/add.svg" @click="AddEvent()" class="add_img">
      </div>
    </div>
  </div>
</template>

<style scoped>
.add_img{ margin: 0 0 1em 0; width: 4em; height: 4em; cursor: pointer; }
.event_head { display: flex; flex-wrap: wrap; }
.event_age{ margin: 0 1em;}
</style>
