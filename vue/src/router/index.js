import { createRouter, createWebHistory } from 'vue-router'
import buda from '../buda.js'

let {services}=buda;

let routes=services.map(s=>({
  path: s.route,
  name: s.code,
  component: () => import(`../views/${s.Code}View.vue`)
}));

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes
});

export default router;
