import buda from './buda.js'

import './assets/main.css'
import { createApp, ref } from 'vue'
import App from './App.vue'
import router from './router'

const app = createApp(App);

app.use(router);

app.config.errorHandler = (err) => {
  console.log(`app.config.errorHandler:`, err);
};

app.config.globalProperties.buda = buda;

// app.component('TodoDeleteButton', TodoDeleteButton)

document.title=buda.site.name;
document.body.style.fontSize = buda.ui.rem + 'px'; 
// console.log('body font size:', window.getComputedStyle(document.body, null).getPropertyValue('font-size'));

let Buda = window.Buda = {
  window_resize: (event)=>{
    let {ui}=buda;
    ui.w = window.innerWidth; ui.h = window.innerHeight;
    let gridBox = document.querySelector('.grid_c'); // console.log('gridBox', gridBox);
    gridBox.style.setProperty('--ColumnCount', Math.round(ui.w/ui.rem/ui.list.item.w));
  },
};
addEventListener("resize", Buda.window_resize);

app.mount('#app');
