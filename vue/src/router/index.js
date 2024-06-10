import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView
    },
    {
      path: '/test',
      name: 'test',
      component: () => import('../views/TestView.vue')
    },
    {
      path: '/jobs',
      name: 'jobs',
      // route level code-splitting
      // this generates a separate chunk (About.[hash].js) for this route
      // which is lazy-loaded when the route is visited.
      component: () => import('../views/JobsView.vue')
    },
    {
      path: '/providers',
      name: 'providers',
      component: () => import('../views/ProvidersView.vue')
    },
    {
      path: '/live',
      name: 'live',
      component: () => import('../views/LiveView.vue')
    },
    {
      path: '/goods',
      name: 'goods',
      component: () => import('../views/GoodsView.vue')
    },
  ],
});

export default router
