<script setup>
import buda from '../buda.js'
import { onMounted } from 'vue'

onMounted(() => {
  Buda.window_resize();
});

</script>

<template>
  <div class="grid_c">
    <div v-for="a in buda.jobs.filter(i=>!i.closed)"  class="block">
      <div class="block_inner">
        <div class="company">{{ a.company }}</div>
        <div>地址：{{ a.location }}</div>
        <div v-if="a.contacts && a.contacts.length" class="contacts_c">
          <div class="field_label">联系人：</div>
          <div class="contacts">
            <div class="contact" v-for="d in a.contacts">
              {{ d }}
            </div>
          </div>
        </div>
        <div>{{ a.summary }}</div>
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
.company{font-weight: bold;}
.detail_image{ width:100%; border-radius: 0.5rem; margin: 1rem 0 0 0; }
.contacts_c{display: flex;}
.contacts{ display:flex; flex-wrap: wrap;}
.contact{ margin: 0 1rem 0 0; }
</style>
