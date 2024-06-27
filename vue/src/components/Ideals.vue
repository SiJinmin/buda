<script setup>
import buda from '../buda.js'
import { onMounted } from 'vue'
import BudaImg from '../comps/Img.vue'

onMounted(() => {
  Buda.window_resize();
});

</script>

<template>
  <div class="grid_c">
    <div v-for="a in buda.ideals.filter(i=>!i.closed)"  class="block">
      <div class="block_inner">
        <RouterLink :to="`/ideal/${a.id}`" class="title">{{ a.title }}</RouterLink>
        <div class="field"><div class="field_title">时间</div><div class="field_content">{{ a.time }}</div></div>
        <div class="field"><div class="field_title">地址</div><div class="field_content">{{ a.location }}</div></div>
        <div v-if="a.contacts && a.contacts.length" class="field">
          <div class="field_title">组织者</div>
          <div class="contacts">
            <div class="rm" v-for="d in a.contacts">
              {{ d }}
            </div>
          </div>
        </div>
        <div class="bm">
          <span class="field_title">活动简介</span>
          {{ a.summary }}具体活动流程请点击绿色标题查看详情。
        </div>
        <BudaImg v-if="a.imgs && a.imgs.length" :imgs="a.imgs.map(name=>({ alt: name, src: `/images/companies/small/${name}`}))" :img_class="{detail_image: true}"/>      
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
