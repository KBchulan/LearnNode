import { createApp } from 'vue'
import App from './App.vue'
import Loading from './components/index'

export const app = createApp(App)

app.use(Loading)
app.mount('#app')

declare module 'vue' {
  interface ComponentCustomProperties {
    $loading: {
      show: () => void
      hide: () => void
    }
  }
}