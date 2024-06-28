<script setup>
import { ref, reactive, computed, onMounted , onUnmounted, nextTick } from 'vue'
import buda from '../buda.js'

const props = defineProps({
  imgs: Array,
  img_class:{
    type: Object,
    default(rawProps) { return { };  }
  },
}); 
const {imgs, img_class}=props; const imgs_len=imgs.length;
let index=ref(0), fullscreen=ref(false), loading=ref(false); let container_class=computed(() => ({ fullscreen: fullscreen.value }) );
const img = computed(() => imgs[index.value] ); let img_src = ref(img.value.src);

async function GetImgBig(img){
  if(img.big) return; if(!img.src.includes("/small/")) {img.big=img.src; return;} img.big=await GetBlobUrl(img.src.replace("/small/", "/")); console.log('got big image: ', img.src);
}
async function GetNextImgBig(){
  for(let i=0;i<imgs_len;i++){ if(!fullscreen.value) break; let img=imgs[i]; if(img.big) continue; await GetImgBig(img); break; }
}
onMounted(async () => { 
  setTimeout(async () => {
    await GetImgBig(img.value); img_src.value=img.value.big; // console.log('big img', img_src.value);
  }, 1000);
});
onUnmounted(()=>{   
});

let disable_touch=false;
async function ClickImg(){
  if(fullscreen.value) return; disable_touch=true; 
  fullscreen.value=true; document.body.style.overflow = 'hidden'; // 隐藏页面滚动条
  await nextTick(); await GetNextImgBig(); disable_touch=false;
}
async function ClickCloseImg(){
  disable_touch=true; fullscreen.value=false; document.body.style.overflow = ''; await nextTick(); disable_touch=false;
}
async function ClickNextImg(type){ 
  disable_touch=true; 
  let v=index.value; if(type=='left') { v--; if(v<0) v=0; } else { v++; if(v>=imgs_len) v=imgs_len-1; }
  index.value=v; let img=imgs[v]; loading.value=true; await GetImgBig(img); loading.value=false; img_src.value=img.big; 
  await nextTick(); await GetNextImgBig(); disable_touch=false;
}

function SwipeImg(type)
{ 
  if(!fullscreen.value || disable_touch) return;
  console.log('swipe', type); ClickNextImg(type);
}
function PressImg(){ console.log('press image'); }
function touchHandler() { console.log('touch'); }
function GetImgBlobUrl(img) {  
  /*const canvas = document.createElement('canvas'); canvas.width = img.width; canvas.height = img.height;
  const ctx = canvas.getContext('2d'); ctx.drawImage(img, 0, 0);
  //const base64 = canvas.toDataURL('image/png'); return base64;
  // 转为blob对象 toBlob方法第一个参数是回调函数，第二个是图片格式默认值image/png，第三个为图片质量
  canvas.toBlob(blob => { 
    const url = URL.createObjectURL(blob);
    //URL.revokeObjectURL(url); 释放URL的内存
  });
  canvas.remove();*/

  
  /*const img_big = new Image();  img_big.setAttribute('crossOrigin', 'anonymous'); img_big.src = big_src;
  img_big.onload = () => {         
    const reader = new FileReader();
    reader.onload = (event) => { const blob=event.target.result; const url = URL.createObjectURL(blob); img.src=url; };
    reader.readAsDataURL(img_big.src);
  };*/
}
async function fetchBlob(url) { const response = await fetch(url); const blob = await response.blob(); return blob; }
async function GetBlobUrl(big_src) {  return URL.createObjectURL(await fetchBlob(big_src)); }

</script>

<template>
  <div :class="container_class">
    <img v-touch:press="PressImg" v-touch:swipe.left="SwipeImg('left')"  v-touch:swipe.right="SwipeImg('right')"  :src="img_src" :alt="img.alt" :title="img.alt" :class="img_class" @click="ClickImg"/>
    <div v-if="fullscreen && loading" class="loading">正在加载图片...</div>
    <div class="screen_bottom_bar" v-if="fullscreen">
      <div @click="ClickNextImg('left')" class="left_img_c flex">
        <div class="flex big_ico_c">
          <div v-if="imgs_len>1 && index>0" class="flex hand">
            <img src="@/assets/ico/left.svg" alt="上一张" title="上一张" class="big_ico_button"/>
          </div>
        </div>
      </div>
      <div class="flexg flex"><div>第{{index+1}}/{{imgs_len}}张</div></div>
      <div @click="ClickNextImg('right')" class="right_img_c flex">
        <div class="flex big_ico_c">
          <div v-if="imgs_len>1 && index<imgs_len-1" class="flex hand">
            <img src="@/assets/ico/right.svg" alt="下一张" title="下一张" class="big_ico_button"/>
          </div>
        </div>
      </div>
    </div>
    <img v-if="fullscreen" src="@/assets/ico/close.svg" class="close_img big_ico_button" alt="关闭全屏" title="关闭全屏" @click="ClickCloseImg"/>    
  </div>
</template>

<style scoped>
</style>
