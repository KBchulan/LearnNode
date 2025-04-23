import { createApp } from 'vue'

import App from './App.vue'
import Slot from './components/Slot.vue'

export const app = createApp(App)
app.component('Slot', Slot)
app.mount('#app')
