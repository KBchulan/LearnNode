import { createApp } from 'vue'
import App from './App.vue'
import did from './components/did.vue'

export const app = createApp(App)
app.component('did', did)
app.mount('#app')
