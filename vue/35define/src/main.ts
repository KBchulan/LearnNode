import { createApp } from 'vue'
import App from './App.vue'
import child from './components/child.vue'

export const app = createApp(App)

app.component('child', child)
app.mount('#app')
