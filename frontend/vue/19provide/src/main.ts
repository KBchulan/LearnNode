import { createApp } from 'vue'

import App from './App.vue'
import father from './components/father.vue'
import son from './components/son.vue'

export const app = createApp(App)

app.component('father', father)
app.component('son', son)
app.mount('#app')
