<script setup>
import buda from './buda.js'
import { onMounted, ref, watch } from 'vue'
import { RouterLink, RouterView, useRouter } from 'vue-router'

const router = useRouter();
router.afterEach((to, from) => { 
  let code = to.name;
  let m=buda.modules.find(s=>s.code==code); if(m){
    buda.page.module=m;
    console.log('route change :', code, m.title); 
  }
});

onMounted(() => {
  // console.log('buda', buda);
  
});

const SiteMenuShowing=ref(false);
function ClickSiteMenu(){ 
  SiteMenuShowing.value = !SiteMenuShowing.value; // console.log('SiteMenuShowing=',SiteMenuShowing.value); 
}
function CloseSiteMenu(){ SiteMenuShowing.value = false; }
</script>

<template>  
  <div @click="CloseSiteMenu">
    <header>
      <div class="site_header_left">
        <img v-if="false" alt="登录" class="ico_button" src="@/assets/ico/login.svg" @click="ClickSiteMenu">        
      </div>
      <div class="site_title_c">
        <img v-if="false" alt="logo" class="logo" src="@/assets/ico/logo.svg"/>
        <RouterLink class="site_title" to="/">{{ buda.community }}</RouterLink>
      </div>
      <div class="site_header_right">
        <img alt="menu" class="ico_button" src="@/assets/ico/menu.svg" @click.stop="ClickSiteMenu">
      </div>
      <div class="site_menu_c" v-if="SiteMenuShowing">
        <RouterLink v-for="a in buda.menu_modules" :to="a.route">{{ a.title }}</RouterLink>
      </div>
    </header>
    <div class="site_content_c">
      <div class="page_title_c">
        <div class="page_title">{{ buda.page.title || buda.page.module.title2 }}</div>
      </div>
      <div v-if="buda.page.msg" class="page_msg">{{ buda.page.msg }}</div>
      <div class="site_content_inner">
        <div class="page_main_des">{{ buda.page.module.des_main }}</div>
        <RouterView />
      </div>
    </div>
  </div>
</template>

<style scoped>
header { padding: 0.6rem 0.4rem 0.8rem 0.4rem; display: flex; align-items: center; font-size: 2rem; line-height: 1;  background-color: var(--color-block); color: var(--color-site-title);  }
.site_header_left { flex-grow:1; font-size: 70%;  }
.site_header_right { flex-grow:1; display:flex; justify-content: flex-end; }
.site_title_c { display:flex; cursor:pointer; }
.site_title { color: var(--color-site-title); }

.logo { display: block; width: 1em; height: 1em; margin: 0 0.2em 0 0; }
.site_menu_c { z-index: 999; position: fixed; right:0; top:0; margin: 2.9rem 0.4rem 0 0.4rem; border-radius: 0.5rem; padding: 1em 0 0 1em; font-size: 60%; background-color: var(--color-site-title);   }
.site_menu_c a { padding: 0 1em 1em 0; display: inline-block; }

.site_content_c { padding: 1rem 0;  }
</style>
