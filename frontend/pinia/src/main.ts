import { createApp, toRaw } from 'vue'
import App from './App.vue'
import { createPinia } from 'pinia'
import type { PiniaPluginContext } from 'pinia'

const setStorage = (key: string, value: any) => {
  localStorage.setItem(key, JSON.stringify(value))
}

const getStorage = (key: string) => {
  return localStorage.getItem(key) ? JSON.parse(localStorage.getItem(key) as string) : {}
}

const __piniaKey__ = 'hx'

// 做一个持久化的插件，因为刷新时会清空，也就是state会发生变化，所以我们选择$subscribe
const piniaPlugin = (options: { key?: string }) => {
  return (context: PiniaPluginContext) => {
    const { store } = context
    const storeKey = `${options.key ?? __piniaKey__}-${store.$id}`
    console.log(storeKey)    

    // 从本地存储恢复状态
    const data = getStorage(storeKey)

    // 如果有存储的数据，则恢复到 store 中
    if (data) {
      store.$patch(data)
    }

    // 监听状态变化，保存到本地存储
    store.$subscribe(() => {
      setStorage(storeKey, toRaw(store.$state))
    })
  }
}

export const app = createApp(App)
export const pinia = createPinia()

// plugins
pinia.use(piniaPlugin({
  key: 'pinia'
}))
app.use(pinia)

// components

// params

app.mount('#app')