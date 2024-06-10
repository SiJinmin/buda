<script setup>
import buda from '../buda.js'
import { onMounted } from 'vue'

buda.page.name='纯素超市';

onMounted(() => {
  Buda.window_resize();
  // console.log('buda.page.name',buda.page.name);
});

</script>

<template>
  <div>
    <div class="service_c">
      <div>以下水果鲜花在{{ buda.community }}范围内免费送货上门，订购时付款，不满意可以换货，也可以退款。</div>
      <div>下午订购，次日上午8点前送达；上午订购，当天下午4点前送达；您还可以指定送货时间，我们将在您指定的时间内送货。</div>
      <div>订购/客服微信：TobeBuda（订购暂时通过微信联系，我们将在一周内完成本页面的订购功能。）</div>
    </div>
    <div class="grid_c">
      <div v-for="a in buda.goods.filter(i=>!i.closed)"  class="block">
        <div class="block_inner2">
          <div class="block_left_img">
            <div v-if="a.pics && a.pics.length" v-for="d in a.pics">
              <img :title="d" :src="`/images/goods/small/${d}`" class="detail_image">
            </div>
          </div>
          <div class="block_right_c">
            <div class="block_right_title">{{ a.name }}</div>
            <div class="block_right_des">{{ a.des }}</div>
            <div class="block_right_money">{{ a.price }} {{ a.price_unit }}</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.service_c{ padding: 0.5rem 2rem 0.5rem 0.5rem; }
.detail_image{ width:100%; border-radius: 0.5rem; margin: 0 0 0 0; }
.contacts_c{display: flex;}
.contacts{ display:flex;}
.contact{ margin: 0 1rem 0 0; }
</style>
