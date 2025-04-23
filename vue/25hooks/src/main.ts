import { createApp } from 'vue'
import App from './App.vue'
import ColorMode from './components/ColorMode.vue'
import A from './components/A.vue'

export const app = createApp(App)
app.component('ColorMode', ColorMode)
app.component('A', A)
app.mount('#app')
