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
  refs:'',
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
function event_content_input(event, ei){
  let reg_year=/([0-9]{1,4})\s*年/;
  let reg_month=/([0-9]{1,2})\s*月/;
  let reg_day=/([0-9]{1,2})\s*日/;
  let v=event.target.value, r, d=life.events[ei].date;
  if((r=reg_year.exec(v)) && r.length>=2) d.year=parseInt(r[1]);
  if((r=reg_month.exec(v)) && r.length>=2) d.month=parseInt(r[1]);
  if((r=reg_day.exec(v)) && r.length>=2) d.day=parseInt(r[1]);
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
    <section class="editor">
      <div class="field_editor">
        <div class="field_editor_label">标题</div>
        <div class="field_editor_content"><BudaInput v-model="life.title" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" :maxh="3"/></div>
      </div>
      <div class="field_editor">
        <div class="field_editor_label">出生时间</div>
        <div class="field_editor_content"><BudaDate v-model:year="life.birth.year" v-model:month="life.birth.month" v-model:day="life.birth.day" :func="save_birth" /></div>      
      </div>  
      <div class="field_editor">
        <div class="field_editor_label">简称</div>
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
            <div class="field_editor_content"><BudaInput v-model="e.content" @input="event_content_input($event, ei)" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save"/></div>
          </div>
        </div>
        <div>
          <img src="@/assets/add.svg" @click="AddEvent()" class="add_img">
        </div>
      </div> 
      <div class="field_editor_multilines">
        <div class="field_editor_label">参考资料</div>
        <div class="field_editor_content"><BudaInput v-model="life.refs" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" /></div>
      </div> 
    </section>
    <section class="preview">

      
    </section>
  </div>
</template>

<style scoped>
.add_img{ margin: 0 0 1em 0; width: 2em; height: 2em; cursor: pointer; }
.event_head { display: flex; flex-wrap: wrap; }
.event_age{ margin: 0 1em;}
</style>
