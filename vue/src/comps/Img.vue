<script setup>
import { ref, reactive, computed, onMounted , onUnmounted } from 'vue'
import buda from '../buda.js'

const props = defineProps({
  imgs: Array,
  img_class:{
    type: Object,
    default(rawProps) { return { };  }
  },
}); 
const {imgs, img_class}=props; 
let index=ref(0), fullscreen=ref(false); let container_class=computed(() => ({ fullscreen: fullscreen.value }) );
const img = computed(() => imgs[index.value] ); let img_src = ref(img.value.src);

async function GetImgBig(img){
  if(img.big) return; if(!img.src.includes("/small/")) {img.big=img.src; return;} img.big=await GetBlobUrl(img.src.replace("/small/", "/"));
}
onMounted(async () => { 
  await GetImgBig(img.value); img_src.value=img.value.big; // console.log('big img', img_src.value);
});
onUnmounted(()=>{   
});


function ClickImg(){
  if(fullscreen.value) return; fullscreen.value=true; document.body.style.overflow = 'hidden'; // 隐藏页面滚动条
}
function ClickCloseImg(){
  fullscreen.value=false; document.body.style.overflow = ''; // 恢复页面滚动条
}
async function ClickNextImg(type){ 
  let v=index.value; if(type=='left') { v--; if(v<0) v=0; } else { v++; if(v>=imgs.length) v=imgs.length-1; }
  index.value=v; let img=imgs[v]; await GetImgBig(img); img_src.value=img.big; 
}

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
    <img :src="img_src" :alt="img.alt" :title="img.alt" :class="img_class" @click="ClickImg"/>
    <img v-if="fullscreen && index>0" src="@/assets/left.svg" alt="前一张" title="前一张" class="left_img" @click="ClickNextImg('left')"/>
    <img v-if="imgs.length>1 && fullscreen && index<imgs.length-1" src="@/assets/right.svg" alt="后一张" title="后一张" class="right_img" @click="ClickNextImg('right')"/>
    <img v-if="fullscreen" src="@/assets/close.svg" class="close_img" alt="关闭全屏" title="关闭全屏" @click="ClickCloseImg"/>
  </div>
</template>

<style scoped>
</style>
