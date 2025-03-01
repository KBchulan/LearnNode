import { createApp } from 'vue'
import App from './App.vue'
import ColorMode from './components/ColorMode.vue'

export const app = createApp(App)
app.component('ColorMode', ColorMode)
app.mount('#app')
