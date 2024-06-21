<script setup>
import buda from '../buda.js'
import { onMounted } from 'vue'

onMounted(() => {
  Buda.window_resize();
});
 
</script>

<template>
  <div class="grid_c">
    <div v-for="a in buda.houses.filter(i=>!i.closed)"  class="block">
      <div class="block_inner">
        <div class="title">{{ a.community.name }}{{a.area}}平米{{ a.fitment }}{{ a.format }}出租，{{ a.price }}元/月</div>
        <div v-if="a.contacts && a.contacts.length" class="contacts_c">
          <div><span class="field_title">联系人</span></div>
          <div class="contacts">
            <div class="contact" v-for="d in a.contacts">
              {{ d }}
            </div>
          </div>
        </div>
        <div>{{ a.devices }}</div>
        <div>第{{ a.floor }}层<span v-if="a.floors_count">/共{{ a.floors_count }}层</span><span v-if="a.lift">，有{{ a.lift }}部电梯</span></div>
        <div class="community">
          <div>{{ a.community.name }}小区位于{{ a.addr }}<span v-if="a.community.birth">，建成于{{ a.community.birth}}，共有{{ a.community.building_count }}栋建筑，{{ a.community.house_count }}套房子，绿化率为{{ a.community.green }}，容积率为{{ a.community.rate }}，物业费按{{ a.community.fee }}元/平方/月收取，物业公司为{{ a.community.manage }}，开发商为{{ a.community.developer }}。 参考售价为{{ a.sale }}元/平方。</span></div>
        </div>
        <div>{{ a.summary }}</div>
        <div v-if="a.detail && a.detail.length" v-for="d in a.detail">
          <img :title="d" :src="`/images/houses/small/${d}`" class="detail_image">
        </div>
        <div v-if="a.env && a.env.length" v-for="d in a.env">
          <img :title="d" :src="`/images/houses/small/${d}`" class="detail_image">
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.title{font-weight: bold;}
.detail_image{ width:100%; border-radius: 0.5rem; margin: 1rem 0 0 0; }
.contacts_c{display: flex;}
</style>
