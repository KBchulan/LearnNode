import { createApp } from 'vue'
import App from './App.vue'

export const app = createApp(App)

// 全局属性，在template中直接使用，但是在ts中使用需要先获取CurrentInstance
app.config.globalProperties.$age = 18
app.config.globalProperties.$filiter = {
  format: (value: string) => {
    return 'hx-' + value + '-filiter'
  }
}

app.config.globalProperties.$mmm = 'huaixi'

declare module 'vue' {
  interface ComponentCustomProperties {
    $age: number
    $filiter: {
      format: (value: string) => string
    }
    $mmm: string
  }
}
app.mount('#app')
