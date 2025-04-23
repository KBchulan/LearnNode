import { createApp } from 'vue'

import App from './App.vue'
import inner from './components/inner.vue'

export const app = createApp(App)
app.component('inner', inner)
app.mount('#app')
