import { createApp } from 'vue'
import App from './App.vue'
import useResize from 'hx-resize'

export const app = createApp(App)
app.use(useResize)
app.mount('#app')
