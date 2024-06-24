<script setup>
import buda from '../buda.js'
import { onMounted } from 'vue'

onMounted(() => {
  Buda.window_resize();
});

</script>

<template>
  <div class="grid_c">
    <div v-for="a in buda.jobs"  class="block">
      <div class="block_inner">
        <div class="title">{{ a.company }}</div>
        <div><span class="field_title">地址</span>{{ a.location }}</div>
        <div v-if="a.contacts && a.contacts.length" class="contacts_c">
          <div><span class="field_title">联系人</span></div>
          <div class="contacts">
            <div class="rm" v-for="d in a.contacts">
              {{ d }} 
            </div>
          </div>
        </div>
        <div>{{ a.summary }}</div>
        <ul>
          <li v-for="(s, index) in a.ServiceItems" :key="index">{{ s }}</li>
        </ul>
        <div v-if="a.detail && a.detail.length" v-for="d in a.detail">
          <img :title="d" :src="`/images/companies/small/${d}`" class="detail_image">
        </div>
        <div v-if="a.env && a.env.length" v-for="d in a.env">
          <img :title="d" :src="`/images/companies/small/${d}`" class="detail_image">
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
