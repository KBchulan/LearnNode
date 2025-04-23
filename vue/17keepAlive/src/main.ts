import { createApp } from 'vue'

import App from './App.vue'

// global component
import inner from './components/inner.vue'
import other from './components/other.vue'

// third party
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'

export const app = createApp(App)
app.use(ElementPlus)
app.component('inner', inner)
app.component('other', other)
app.mount('#app')
