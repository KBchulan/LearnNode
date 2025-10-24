import { fileURLToPath, URL } from 'node:url'

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'
import path from 'node:path'

console.log(process.cwd())
console.log(import.meta.dirname)
console.log(fileURLToPath(new URL('.', import.meta.url)))
console.log(path.resolve(__dirname, '.'))

// https://vite.dev/config/
export default defineConfig({
  plugins: [
    vue({
      template: {
        compilerOptions: {
          isCustomElement: (tag) => tag.includes('hx-'),  // 此时单文件组件需要命名为.ce.vue，否则会报错，被命名的文件都会被识别为自定义元素
        },
      },
    }),
    vueDevTools(),
  ],
  css: {
    preprocessorOptions: {
      scss: {
        additionalData: '@use "@/bem.scss" as *;',
      },
    },
  },
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
    },
  },
})

