<script setup>
import { onMounted } from 'vue'
import BudaInput from '../comps/Input.vue'

const year = defineModel('year');
const month = defineModel('month');
const day = defineModel('day');
const props = defineProps({
  func: Function,
}); 
const { func }=props;

onMounted(() => {
  
});

</script>

<template>
  <div>
    <BudaInput v-model="year" :func="func" :input_classes="{field_editor_input_4chars:true, center:true,}"/><div class="field_editor_content_label">年</div>
    <BudaInput v-model="month" :func="func" :input_classes="{field_editor_input_2chars:true, center:true,}"/><div class="field_editor_content_label">月</div>
    <BudaInput v-model="day" :func="func" :input_classes="{field_editor_input_2chars:true, center:true,}"/><div class="field_editor_content_label">日</div>
  </div>
</template>

<style scoped>

</style>
