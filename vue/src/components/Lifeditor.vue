<script setup>
import buda from '../buda.js'
import BudaDate from '../comps/Date.vue'
import BudaInput from '../comps/Input.vue'

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

// 自由、开放、透明、免费、去中心化、全民协作思想的领军者、核心力量
function save(){
  localStorage.setItem("life", JSON.stringify(life));
  console.log('saved', new Date());
}

function Input(){}


onMounted(() => {
 // Buda.window_resize();
});

</script>

<template>
  <div class="page_comp_small_padding">
    <div class="field_editor">
      <div class="field_editor_label">标题</div>
      <div class="field_editor_content"><BudaInput v-model="life.title" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" :maxh="3" input_id="textarea_title"/></div>
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
      <div class="field_editor_content"><BudaInput v-model="life.summary" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" input_id="textarea_summary"/></div>
    </div> 
    <div>事件</div>
    <div>
      <div v-for="(e, ei) in life.events" class="field_editor_verticle">
        <div>
          <BudaDate v-model:year="e.date.year" v-model:month="e.date.month" v-model:day="e.date.day" @input="Input" />
        </div>
        <div class="field_editor_content"><BudaInput v-model="e.content" type="textarea" :input_classes="{field_editor_input_long:true}" :func="save" :minh="4" :input_id="`textarea_event_${ei}_content`"/></div>
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
