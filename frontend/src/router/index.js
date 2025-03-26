// Composables
import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  { path: '/', name: 'Home', component: () => import('@/views/Home.vue'), meta:{ transition: 'fade'} }, 
  { path: '/stream', name: 'Stream', component: () => import('@/views/Stream.vue'), meta:{ transition: 'fade'} } , 
  { path: '/snapshot', name: 'Snapshot', component: () => import('@/views/Snapshot.vue'), meta:{ transition: 'fade'} } 
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes,
})

export default router
