import { createRouter, createWebHistory } from 'vue-router'
import buda from '../buda.js'

let {modules}=buda;

let routes=modules.map(s=>({
  path: s.route,
  name: s.code,
  component: () => import(`../views/${s.Code}View.vue`)
}));

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes
});

export default router;
