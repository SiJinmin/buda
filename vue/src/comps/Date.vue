<script setup>
import { onMounted } from 'vue'

const year = defineModel('year');
const month = defineModel('month');
const day = defineModel('day');

onMounted(() => {
  
});

</script>

<template>
  <div>
    <input v-model="year" class="field_editor_input_4chars center"/><div class="field_editor_content_label">年</div>
    <input v-model="month" class="field_editor_input_2chars center"/><div class="field_editor_content_label">月</div>
    <input v-model="day" class="field_editor_input_2chars center"/><div class="field_editor_content_label">日</div>
  </div>
</template>

<style scoped>

</style>
