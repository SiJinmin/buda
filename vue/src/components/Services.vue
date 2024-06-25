<script setup>
import buda from '../buda.js'
import { onMounted } from 'vue'

onMounted(() => {
  Buda.window_resize();
});

</script>

<template>
  <div class="grid_c">
    <div class="block" v-for="a in buda.live_modules">
      <div class="block_inner">
        <RouterLink :to="a.route">{{ a.title2 }}</RouterLink>
        <div class="block_content">{{ a.des }}</div>
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
