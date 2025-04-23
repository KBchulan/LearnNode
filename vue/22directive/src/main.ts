import { createApp } from 'vue'
import App from './App.vue'
import did from './components/did.vue'
import simble from './components/simble.vue'

export const app = createApp(App)
app.component('did', did)
app.component('simble', simble)
app.mount('#app')
