import { createApp } from 'vue'

import App from './App.vue'
import Skeleton from './components/skeleton.vue'

export const app = createApp(App)
app.component('Skeleton', Skeleton)
app.mount('#app')
