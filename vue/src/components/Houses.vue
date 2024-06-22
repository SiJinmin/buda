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
        <div>房子位于第{{ a.floor }}层<span v-if="a.floors_count">/共{{ a.floors_count }}层</span><span v-if="a.lift">，有{{ a.lift }}部电梯</span></div>
        <div class="community">
          <div>{{ a.community.name }}小区位于{{ a.addr }}<span v-if="a.community.birth">，建成于{{ a.community.birth}}，共有{{ a.community.building_count }}栋建筑，{{ a.community.house_count }}套房子，绿化率为{{ a.community.green }}，容积率为{{ a.community.rate }}，物业费按{{ a.community.fee }}元/平方/月收取，物业公司为{{ a.community.manage }}，开发商为{{ a.community.developer }}。 参考售价为{{ a.sale }}元/平方。</span></div>
        </div>
        <div>{{ a.summary }}</div>
        <BudaImg v-if="a.imgs && a.imgs.length" :imgs="a.imgs.map(name=>({ alt: name, src: `/images/houses/small/${name}`}))" :img_class="{detail_image: true}"/>  
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
