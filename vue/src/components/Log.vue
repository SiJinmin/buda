<script setup>
import { ref, reactive, computed, onMounted , onUnmounted, nextTick } from 'vue'

let files=ref({}), file_index=ref(-1), file=computed(() => files.value[file_index.value]);

function make_record(str, f)
{
  str = String.Buda_TextToHtml(str,true);
  let reg=new RegExp("(?<whole>(?<start>\\[)([0-9]{4})-([0-9]{2})-([0-9]{2})[\u0020]([0-9]{2,2}):([0-9]{2,2}):([0-9]{2,2})(\\])(\\[)([0-9.]+)(?<end>\\]))", "g");
  let r =  [...str.matchAll(reg)]; r=r[0]; let time=r.groups.whole, content=str.substring(time.length).replace(/\n/g, '<br>');
  r=`<div class='log_record'><div class='log_time'>${time}</div><div classs='log_record_content'>${content}</div></div>`;
  let html=r; //  console.log(f.html);
  return {time, content, html};
}
async function click_file(fi)
{
  file_index.value=fi; let f=file.value; if(f.content) return; let url=`/api/log_file/${f.name}`; console.log('fetching', url);
  let c = f.content =  await (await fetch(url)).text(); f.size=Math.floor(f.content.length/1024); 
  if(!f.content) f.content = "（无内容）"; // console.log('content', f.content);
  let reg=new RegExp("\\[[0-9]{4}-[0-9]{2}-[0-9]{2}[\u0020][0-9]{2,2}:[0-9]{2,2}:[0-9]{2,2}\\]\\[[0-9.]+\\]", "g");
  let match, start=-1, records=f.records=[];  while ((match = reg.exec(c)) !== null) 
  { if(start>=0) records.push(make_record(c.substring(start, match.index), f)); start=match.index; }
  records.push(make_record(c.substring(start), f)); // console.log(f);
  return;
  
}

onMounted(async () => {
  let files2 = await fetch("/api/log_files"); files2 = await files2.text(); 
  files.value=files2.split('\n').filter(f=>f).sort((a, b)=> a<b?1:-1).map(f=>({name: f}));
});

</script>

<template>
  <div>
    <div class="flex bm">
      <div class="field_editor_search"><input class="field_editor_input_search"><button class="button">查看</button></div>
    </div>
    <div class="flex0">
      <div>
        <div class="name_list_item" v-for="(f, fi) in files">
          <div class="link" @click="click_file(fi)">{{ f.name }}<span v-if="f.size">&nbsp;[{{ f.size }}K]</span></div>
        </div>
      </div>
      <div v-if="file_index>=0" class="flexg">
        <div class="title flex">{{ file.name }}</div>
        <div class="pad">
          <div v-for="record in file.records" v-html="record.html"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
