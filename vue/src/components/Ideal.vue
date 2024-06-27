<script setup>
import buda from '../buda.js'
import { ref, onMounted } from 'vue'
import { useRoute } from 'vue-router'
import BudaImg from '../comps/Img.vue'

const route = useRoute(); let ideal_id=route.params.id, ideal=buda.ideals.find(i=>i.id==ideal_id); let a=ref(ideal);
buda.page.title=ideal.title;



onMounted(() => {
  
});

</script>

<template>
      <div class="page_comp_small_padding">
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
          {{ a.summary }}
        </div>
        <div class="bm">
          <span class="field_title">活动流程</span>
          <div>
            <div v-for="(item, i) in a.flow">{{i+1}}. {{ item }}</div>
          </div>
        </div>
        <div>
          <div>以下是活动地点的照片，{{ a.thanks }}</div>
          <div v-if="a.imgs && a.imgs.length" >
            <img v-for="img in a.imgs" :src="`/images/companies/${img}`" class="detail_page_img"/>
          </div>
        </div>
      </div>
</template>

<style scoped>
</style>
