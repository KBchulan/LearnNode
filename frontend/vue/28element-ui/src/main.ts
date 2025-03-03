import { createApp } from 'vue'

import App from './App.vue'
// 我们这里演示一下element-ui，其实还有ant-design-vue，vant等
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'

export const app = createApp(App)

app.use(ElementPlus)
app.mount('#app')
