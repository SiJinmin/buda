<script setup>
import { ref, reactive, computed, onMounted , onUnmounted, nextTick } from 'vue'

let files=ref({}), file_index=ref(-1), file=computed(() => files.value[file_index.value]);
let modes=['raw', 'align', 'summary'], mode=ref('raw'); // align, summary

let reg_time_fast=new RegExp("\\[[0-9]{4}-[0-9]{2}-[0-9]{2}[\u0020][0-9]{2,2}:[0-9]{2,2}:[0-9]{2,2}\\]\\[[0-9.]+\\]", "g");
let reg_time=new RegExp("(?<whole>(?<start>\\[)([0-9]{4})-([0-9]{2})-([0-9]{2})[\u0020]([0-9]{2,2}):([0-9]{2,2}):([0-9]{2,2})(\\])(\\[)([0-9.]+)(?<end>\\]))", "g");
let reg_ip=/1 --- server socket accepted a client connection:  ([0-9\.]+)/g;

let req=null;
function make_req(ip, start_time){ req={ ip, start_time, user_agent:'', url:'', sent:'', end_time:'', records:[], }; }
function make_record(str, f)
{
  let r =  [...str.matchAll(reg_time)][0]; // as str is get by time, so r.length must be 1  
  let time=r.groups.whole, content=str.substring(time.length+1), content_html=String.Buda_TextToHtml(content);
  let record = {time, content, content_html}; f.records.push(record);return;

  let user_agent_start,  ip;
  if((ip=[...content.matchAll(reg_ip)]).length)
  {
    ip=ip[0][1]; make_req(ip, time); console.log('ip', ip);
  }
  else if(content.indexOf("[received")==0 && ((user_agent_start=content.indexOf("user-agent: "))>0 || (user_agent_start=content.indexOf("User-Agent: "))>0))
  {
    req.user_agent=content.substring(user_agent_start, content.indexOf('\n', user_agent_start));
  } 
  else if(content.indexOf('parse http request succeed: ')==0)
  {
    req.url=content.substring('parse http request succeed: '.length);
  }
  else if(content.indexOf('[sent to client ')==0)
  {
    req.sent=content.substring('[sent to client '.length);
  }
  else if(content.indexOf("waiting for next client ..."==0))
  {
    req.end_time=time;
  }

   if(req==null) f.items.push(record); req.records.push(record); return record;
}
async function click_file(fi)
{
  file_index.value=fi; let f=file.value; if(f.content) return; let url=`/api/log_file/${f.name}`; console.log('fetching', url);
  let c = f.content =  await (await fetch(url)).text(); 
  if(!c) {c = f.content = ''; console.log('file has no content'); return;  } 
  f.size=Math.floor(c.length/1024); f.content_html=String.Buda_TextToHtml(c);
  
  let match, start=-1;
  while ((match = reg_time_fast.exec(c)) !== null) 
  { 
    if(start==-1 && match.index!=0) { console.log('file content not starts with time', c); return; } 
    if(start>=0) make_record(c.substring(start, match.index), f); 
    start=match.index; 
  }
  if(start>=0) make_record(c.substring(start), f); else console.log('no records found: ', c);  
}

onMounted(async () => {
  let files2 = await fetch("/api/log_files"); files2 = await files2.text(); 
  files.value=files2.split('\n').filter(f=>f).sort((a, b)=> a<b?1:-1).map(f=>({name: f, content:'', size: 0, content_html:'', records:[], items:[], }));
});

</script>

<template>
  <div>
    <div class="flex bm">
      <div class="field_editor_search"><input class="field_editor_input_search"><button class="button">搜索</button></div>
    </div>
    <div class="flex0">
      <div class="files_c">
        <div class="name_list_item" v-for="(f, fi) in files">
          <div class="link nowrap" @click="click_file(fi)">{{ f.name }}<span v-if="f.size">&nbsp;[{{ f.size }}K]</span></div>
        </div>
      </div>
      <div class="file_detail flexg">
        <div class="topbar flex">
          <div class="radio_list flex">
            <div v-for="m in modes" class="radio_list_item">
              <input type="radio" :value="m" v-model="mode" />
              <label>{{ m }}</label>
            </div>
          </div>
        </div>
        <div v-if="file_index>=0">
          <div class="title flex pad">{{ file.name }}</div>
          <div class="pad">
            <div v-if="mode=='raw'">
              <div v-html="file.content_html"></div>
            </div>
            <div v-else-if="mode=='align'">   
              <div v-if="file.records.length">
                <div>
                  <div v-for="record in file.records" class='log_record'>
                    <div class='log_time'>{{ record.time }}</div>
                    <div classs='log_record_content'><div v-html="record.content_html"></div></div>
                  </div>
                </div>
              </div>         
            </div>
            <div v-else>
            </div>
          </div>
        </div> 
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
