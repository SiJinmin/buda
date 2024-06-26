<script setup>
import { onMounted, ref } from 'vue'

let files=ref([]), file_name=ref(''), file_content=ref('');

async function click_file(file)
{
  file_name.value=file;
  let c =  await (await fetch(`/api/log_file/${file}`)).text(); if(!c) c ="（无内容）";
  c= String.Buda_TextToHtml(c);
  file_content.value = c;
}

onMounted(async () => {
  let files2 = await fetch("/api/log_files"); files2 = await files2.text(); files.value=files2.split('\n').filter(f=>f).sort((a, b)=> a<b?1:-1);

  
});

</script>

<template>
  <div>
    <div class="flex bm">
      <div class="field_editor_search"><input class="field_editor_input_search"><button class="button">查看</button></div>
    </div>
    <div class="flex0">
      <div>
        <div class="name_list_item" v-for="file in files"><div class="link" @click="click_file(file)">{{ file }}</div></div>
      </div>
      <div class="flexg">
        <div class="title flex">{{ file_name }}</div>
        <div class="pad">
          <div v-html="file_content"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
