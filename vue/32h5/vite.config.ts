import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { postcssPxToViewport } from './plugins/postcss-px-to-viewport'
// https://vite.dev/config/
export default defineConfig({
  plugins: [vue()],
  css: {
    postcss: {
      plugins: [postcssPxToViewport()]
    }
  }
})
