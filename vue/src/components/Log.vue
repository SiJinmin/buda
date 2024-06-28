<script setup>
import { ref, reactive, computed, onMounted , onUnmounted, nextTick } from 'vue'
import buda from '../buda.js'

let files=ref([]), file_index=ref(-1), file=computed(() => files.value[file_index.value]);
let modes=['raw', 'records', 'requests', 'users'], mode=ref('raw'); 


onMounted(async () => {
  let files2 = await fetch("/api/log_files"); files2 = await files2.text(); 
  files.value=files2.split('\n').filter(f=>f).sort((a, b)=> a<b?1:-1).map(f=>({name: f, content:'', size: 0, content_html:'', records:[], reqs:[], users:[] }));
});


let reg_time_fast=new RegExp("\\[[0-9]{4}-[0-9]{2}-[0-9]{2}[\u0020][0-9]{2,2}:[0-9]{2,2}:[0-9]{2,2}\\]\\[[0-9.]+\\]", "g");
let reg_time=new RegExp("(?<whole>\\[(?<time>([0-9]{4})-([0-9]{2})-([0-9]{2})[\u0020]([0-9]{2,2}):([0-9]{2,2}):([0-9]{2,2}))\\]\\[(?<sep>[0-9.]+)\\])", "g");
let reg_ip=/--- server socket accepted a client connection:  ([0-9\.]+):([0-9]+)/g;


async function click_file(fi)
{
  file_index.value=fi; let f=file.value; if(f.content) return; // cache content
  let url=`/api/log_file/${f.name}`; console.log('fetching', url);
  let c = f.content =  await (await fetch(url)).text(); if(!c) { buda.page.msg = ('file has no content'); return;  } 
  f.size=Math.floor(c.length/1024); f.content_html=String.Buda_TextToHtml(c);
  
  let match, start=-1;
  while ((match = reg_time_fast.exec(c)) !== null) 
  { 
    if(start==-1 && match.index!=0) { buda.page.msg='file content do not starts with time'; mode.value='raw'; return; } 
    if(start>=0) make_record(c.substring(start, match.index), f); 
    start=match.index; 
  }
  if(start>=0) {make_record(c.substring(start), f); make_reqs(); }
  else { buda.page.msg = 'no records found in file content'; mode.value='raw'; } 
}

function make_sep_text(sep){ let sep_text; if(sep<1) sep_text=(Math.floor(sep*1000000)+' usec'); else sep_text=Math.floor(sep)+' sec'; return sep_text; }
function make_record(str, f)
{
  let r =  [...str.matchAll(reg_time)][0]; // as str is get by time, so r.length must be 1  
  let {whole, time, sep}=r.groups, content=str.substring(whole.length+1), content_html=String.Buda_TextToHtml(content);
  let parts=sep.split('.'), seconds=parseInt(parts[0]), nseconds=parseInt(parts[1]); sep=seconds+nseconds/1000000000;
  let record = {time, sep, sep_text: make_sep_text(sep), content, content_html}; f.records.push(record);
}


function make_req(ip, port, start_time, no){ return { no, ip, port, start_time, user_agent:'', url:'', sent:'', end_time:'', sep:0, records:[], }; }
function make_reqs()
{
  let {records, reqs} = file.value; let req=null, user_agent_start=null,  ip=null, sep=0, seps=0, freetimes=0, req_no=0;
  for(let record of records)
  {    
    let {time, content}=record;
    if((ip=[...content.matchAll(reg_ip)]).length)
    {
      freetimes+=record.sep;
      if(req) { buda.page.msg='pre req is not ended.'; return; }
      ip=ip[0]; req = make_req(ip[1], ip[2], time, ++req_no); // console.log('ip', ip);
    }    
    else 
    {
      if(req) {sep+=record.sep; } else freetimes+=record.sep;
      if(content.indexOf("waiting for next client ...")==0)
      {
        if(!req) { buda.page.msg='got req end while req is null.' + time + content; return; }
        else{ req.end_time=time; req.sep=sep; seps+=sep; req.sep_text=make_sep_text(sep); reqs.push(req); req=null; sep=0; }
      }
      else if(content.indexOf("[received")==0 && ((user_agent_start=content.indexOf("user-agent: "))>0 || (user_agent_start=content.indexOf("User-Agent: "))>0))
      {
        if(!req) { buda.page.msg='got user_agent while req is null.' + time; return; }
        req.user_agent=content.substring(user_agent_start, content.indexOf('\n', user_agent_start));
      } 
      else if(content.indexOf('parse http request succeed: ')==0)
      {
        if(!req) { buda.page.msg='got url while req is null.' + time; return; }
        req.url=decodeURIComponent(content.substring('parse http request succeed: '.length));
      }
      else if(content.indexOf('[sent to client ')==0)
      {
        if(!req) { buda.page.msg='got sent while req is null.' + time; return; }
        req.sent=content.substring('[sent to client '.length); req.sent='['+req.sent;
      }
    }    
  }

  let v=file.value, rc=reqs.length; if(rc)
  {
    // summary of file
    v.process_time=seps; v.process_time_per_req=seps/rc; v.process_time_per_req_text=make_sep_text(seps/rc);
    v.freetimes=freetimes; v.free_degree=Math.floor(freetimes/seps);

    //setup next for requests
    let req_pre=null; for(let req of reqs)
    {
      if(req_pre){ if(req_pre.user_agent==req.user_agent && req_pre.ip==req.ip) req.next=true;    }
      req_pre=req;
    }

    //make users
    let reqs2=[...reqs]; reqs2.sort((a, b)=>{ if(a.user_agent!=b.user_agent) return a.user_agent>b.user_agent?1:-1; else return a.no-b.no;  });
    //setup next for requests
    let req2_pre=null; for(let req of reqs2)
    {
      if(req2_pre){ if(req2_pre.user_agent==req.user_agent) { if(req2_pre.ip==req.ip) req.next2=true; else req.nextu=true; }    }
      req2_pre=req;
    }
    v.users=reqs2;
  }
}

function click_hide_user(req, ri)
{
  req.hide=!req.hide; let {users}=file.value, uc=users.length, r;
  while(++ri<uc)
  {
    r = users[ri]; if(r.next2 || r.nextu) r.hide_next=req.hide; else break;
  }
}

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
          <div class="flex">{{ file.records.length }}条日志 = {{ file.reqs.length }}个请求</div>
          <div class="flex">平均每个请求{{ Math.floor( file.records.length / file.reqs.length )}}条日志、耗时{{ file.process_time_per_req_text }}，空置率{{ file.free_degree }}</div>
          <div class="pad">
            <div v-if="mode=='raw'">
              <div v-html="file.content_html"></div>
            </div>
            <div v-else-if="mode=='records'">   
              <div v-if="file.records.length">
                <div v-for="record in file.records" :class='{log_record:true, log_block_start: record.sep>=1 }'>
                  <div :class='{log_time:true, title: record.sep>=1}'>{{ record.time }} [{{ record.sep_text }}]</div>
                  <div classs='log_record_content'><div v-html="record.content_html"></div></div>
                </div>
              </div>         
            </div>
            <div v-else-if="mode=='requests'">
              <div v-if="file.reqs.length">
                <div v-for="req in file.reqs" :class='{log_req:true, log_block_start: !req.next, highlight: req.sep>=1 }'>
                  <div class="title" v-if="!req.next"><div class="log_ip">{{ req.ip }}</div> {{ req.user_agent }}</div>
                  <div><span class="title">: {{ req.port }}</span><span v-if="true"> [{{ req.start_time }}]</span><span v-if="req.start_time!=req.end_time"> - [{{ req.end_time }}] </span>({{ req.sep_text }})  {{ req.url }}  {{ req.sent }}</div>
                </div>
              </div> 
            </div>
            <div v-else-if="mode=='users'">
              <div v-if="file.users.length">
                <div v-for="(req, ri) in file.users">
                  <div v-if="!req.hide_next" :class='{log_req:true, log_block_start: !req.next2, log_block_start2: req.nextu, highlight: req.sep>=1 }'>
                    <div class="title flex0" v-if="!req.next2">
                      <div class="log_ip">{{ req.ip }}</div><div class="flexg"><div v-if="!req.nextu" class="title">{{ req.user_agent }}</div></div>
                      <div v-if="!req.nextu" @click="click_hide_user(req, ri)" class="lm"><img v-if="!req.hide" class="ico_button" src="@/assets/ico/up.svg"/><img v-else class="ico_button" src="@/assets/ico/down.svg"/></div>                    
                    </div>
                    <div>
                      <span class="title">: {{ req.port }}</span><span v-if="true"> [{{ req.start_time }}]</span><span v-if="req.start_time!=req.end_time"> - [{{ req.end_time }}] </span>({{ req.sep_text }})  {{ req.url }}  {{ req.sent }}
                    </div>
                  </div>
                </div>
              </div> 
            </div>
          </div>
        </div> 
      </div>
    </div>
  </div>
</template>

<style scoped>
</style>
