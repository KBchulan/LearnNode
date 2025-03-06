// 开发环境的插件electron
// 因为我们平时需要挂起dev，同时还需要打开一个新终端，这个插件帮助我们在同一个终端启动这两个服务
import type { Plugin } from 'vite'
import type { AddressInfo } from 'net'
import { spawn } from 'child_process'
import { buildSync } from 'esbuild'
import path from 'path'
import electron from 'electron'
import fs from 'node:fs'

const build = () => {
  buildSync({
    entryPoints: ['src/background.ts'],
    outfile: 'dist/background.js',
    bundle: true,
    platform: 'node',
    target: 'node16',
    format: 'esm', // 指定输出为 ES 模块格式
    external: ['electron']
  })
}

export const ElectronDevPlugin = (): Plugin => {
  return {
    name: 'electron-dev',
    configureServer(server) {
      build()
      server?.httpServer?.once('listening', () => {
        const addressInfo = server?.httpServer?.address() as AddressInfo
        const url = `http://localhost:${addressInfo.port}`

        // 通过spawn传递url
        let ElectronProcess = spawn(electron + '', ['dist/background.js', url])
        fs.watchFile('src/background.ts', () => {
          build()
          ElectronProcess.kill()
          ElectronProcess = spawn(electron + '', ['dist/background.js', url])
        })
      })
    }
  }
}