import { defineConfig } from 'vite'

// 定义Vite配置
export default defineConfig({
  build: {
    lib: {
      // 指定库的入口文件
      entry: './src/index.ts',
      // 指定库的名称，在UMD构建模式下，库的名称将作为全局变量名
      name: 'useResize'
    },
    rollupOptions: {
      // 声明外部依赖，不会被打包进库中
      external: ['vue'],
      output: {
        // 在UMD构建模式下为外部依赖提供全局变量
        globals: {
          vue: 'Vue'  // 当我的库在浏览器中运行并需要 vue 时，去找全局变量 Vue
        }
      }
    }
  }
})