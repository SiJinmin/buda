<script setup>
import buda from '../buda.js'
import BudaDate from '../comps/Date.vue'
import BudaInput from '../comps/Input.vue'
import { ref, reactive, computed, onMounted } from 'vue'


let life, lives, lifeditor;


function AEvent(year, month, day)
{
  return {
    date:{ year, month, day },
    content: '',
    lock_date: false,
  };
}
function ALife()
{
  return {
    name:"",
    title:"",
    birth:{ year:1900, month: 1, day: 1 },
    summary:'',
    events:[ AEvent(1900,1,1), ],
    refs:'',
  };
}


lifeditor=localStorage.getItem("lifeditor"); if(!lifeditor) 
{
  lives=localStorage.getItem("lives"); if(!lives) 
  {
    life=localStorage.getItem("life"); if(!life) life=ALife(); else life = JSON.parse(life);
    lives=[ life ];
  } 
  else lives = JSON.parse(lives);
  lifeditor = { cindex:0, lives };
} else lifeditor = JSON.parse(lifeditor);
lifeditor = reactive(lifeditor); ({lives}=lifeditor);
life = computed(() => lives[lifeditor.cindex]); 


function save()
{
  localStorage.setItem("lifeditor", JSON.stringify(lifeditor)); console.log('saved', new Date());
}
function Age(e){ let f=life.value; return e.date.year-f.birth.year;  }

function select_life(index){ lifeditor.cindex=index; save(); }
function event_content_input(event, ei)
{
  let f=life.value; let e=f.events[ei]; if(!e.lock_date)
  {
    let reg_year=/([0-9]{1,4})\s*年/;
    let reg_month=/([0-9]{1,2})\s*月/;
    let reg_day=/([0-9]{1,2})\s*日/;
    let v=event.target.value, r, d=e.date;
    if((r=reg_year.exec(v)) && r.length>=2) d.year=parseInt(r[1]);
    if((r=reg_month.exec(v)) && r.length>=2) d.month=parseInt(r[1]);
    if((r=reg_day.exec(v)) && r.length>=2) d.day=parseInt(r[1]);
  } 
}
function AddEvent(nexte, nextei)
{
  let f=life.value; let year, {events}=f;
  let next=nexte, pre=(nextei>0?events[nextei-1]:null);
  if(!next) year=pre.date.year+1; else if(!pre) year=next.date.year-1; else year=Math.floor((pre.date.year+next.date.year)/2);
  events.splice(nextei, 0, AEvent(year, '', ''));
  save();
}
function DeleteEvent(ei)
{
  if(!confirm("确定删除本事件？")) return;
  let {events}=life;
  events.splice(ei, 1);
  save();
}
function NewLife()
{
  lives.push(ALife()); save(); lifeditor.cindex = lifeditor.lives.length-1;
}


onMounted(() => 
{
 // Buda.window_resize();
  let textareas=document.getElementsByClassName('auto_height'); for(let textarea of textareas)
  { Buda.dom.setAreaH(textarea, buda.ui.rem, 0,null, 0, 2);  }   
});


</script>

<template>
  <div class="page_comp_small_padding">
    <section class="list pad">
      <div class="block" v-for="(a, i) in lives"><div class="button" @click="select_life(i)">{{  a.name  }}</div></div>
      <div class="block"><div class="button" @click="NewLife">+</div></div>
    </section>
    <section class="editor">
      <div class="field_editor">
        <div class="field_editor_label">标题</div>
        <div class="field_editor_content"><BudaInput v-model="life.title" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" :maxh="3"/></div>
      </div>
      <div class="field_editor">
        <div class="field_editor_label">出生时间</div>
        <div class="field_editor_content"><BudaDate v-model:year="life.birth.year" v-model:month="life.birth.month" v-model:day="life.birth.day" :func="save" /></div>      
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
            <img src="@/assets/ico/add.svg" @click="AddEvent(e, ei)" class="mid_ico_button bm">
          </div>
          <div>
            <div class="event_head">
              <BudaDate v-model:year="e.date.year" v-model:month="e.date.month" v-model:day="e.date.day" :func="save" />
              <img v-if="e.lock_date" title="日期已被锁定，不再提取内容中的日期" src="@/assets/ico/lock_color.svg" class="ico_button" @click="e.lock_date=!e.lock_date" />
              <img v-else  title="从输入内容中自动提取日期" src="@/assets/ico/unlock.svg" class="ico_button" @click="e.lock_date=!e.lock_date" />
              <div class="event_age">{{ Age(e) }}岁</div>
              <div class="sep_div"></div>
              <img title="删除本事件" src="@/assets/ico/delete_left.svg" class="mid_ico_button" @click="DeleteEvent(ei)" />
            </div>
            <div class="field_editor_content"><BudaInput v-model="e.content" @input="event_content_input($event, ei)" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save"/></div>
          </div>
        </div>
        <div>
          <img src="@/assets/ico/add.svg" @click="AddEvent(null, life.events.length)" class="mid_ico_button bm">
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
.sep_div{flex-grow:1;}
.event_head { display: flex; flex-wrap: wrap; align-items: center;}
.event_age{ margin: 0 1em;}
</style>
