import { createApp } from 'vue'

import App from './App.vue'
import ELementPlus from 'element-plus'
import 'element-plus/dist/index.css'

export const app = createApp(App)

app.use(ELementPlus)
app.mount('#app')
