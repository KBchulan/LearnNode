import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'

// https://vite.dev/config/
export default defineConfig({
  plugins: [vue(), vueDevTools()],
  css: {
    preprocessorOptions: {
      scss: {
        additionalData: '@use "@/styles/bem.scss" as *;'
      }
    }
  },
  resolve: {
    alias: {
      '@': process.cwd() + '/src'
    }
  }
})
