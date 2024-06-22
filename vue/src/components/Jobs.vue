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
    <div v-for="a in buda.jobs.filter(i=>!i.closed)"  class="block">
      <div class="block_inner">
        <div class="title">{{ a.company }}</div>
        <div><span class="field_title">地址</span>{{ a.location }}</div>
        <div v-if="a.contacts && a.contacts.length" class="contacts_c">
          <div><span class="field_title">联系人</span></div>
          <div class="contacts">
            <div class="contact" v-for="d in a.contacts">
              {{ d }}
            </div>
          </div>
        </div>
        <div class="detail_summary">{{ a.summary }}</div>
        <BudaImg v-if="a.imgs && a.imgs.length" :imgs="a.imgs.map(name=>({ alt: name, src: `/images/companies/small/${name}`}))" :img_class="{detail_image: true}"/>      
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
