import { createApp } from 'vue'
import App from './App.vue'
import model from './components/model.vue'

export const app = createApp(App)
app.component('model', model)
app.mount('#app')
