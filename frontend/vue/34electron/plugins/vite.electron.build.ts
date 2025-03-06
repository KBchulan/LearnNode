// 生产环境的插件electron

import type { Plugin } from 'vite'
import { buildSync } from 'esbuild'
import fs from 'node:fs'
import * as ElectronBuilder from 'electron-builder'

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

export const ElectronBuildPlugin = (): Plugin => {
  return {
    name: 'electron-build',
    closeBundle() {
      build()
      // electron-builder 的配置
      const json = JSON.parse(fs.readFileSync('package.json', 'utf-8'))
      json.main = 'background.js'
      fs.writeFileSync('dist/package.json', JSON.stringify(json, null, 2))
      
      // 复制编译后的background.js到dist目录根部
      fs.copyFileSync('dist/background.js', 'dist/index.js')

      fs.mkdirSync('dist/node_modules', { recursive: true })
      ElectronBuilder.build({
        config: {
          directories: {
            output: 'release',
            app: 'dist'
          },
          files: [
            "**/*",
            "!**/node_modules/*/{CHANGELOG.md,README.md,README,readme.md,readme}",
            "!**/node_modules/*/{test,__tests__,tests,powered-test,example,examples}",
            "!**/node_modules/*.d.ts",
            "!**/node_modules/.bin",
            "!**/*.{iml,o,hprof,orig,pyc,pyo,rbc,swp,csproj,sln,xproj}",
            "!.editorconfig",
            "!**/._*",
            "!**/{.DS_Store,.git,.hg,.svn,CVS,RCS,SCCS,.gitignore,.gitattributes}",
            "!**/{__pycache__,thumbs.db,.flowconfig,.idea,.vs,.nyc_output}",
            "!**/{appveyor.yml,.travis.yml,circle.yml}",
            "!**/{npm-debug.log,yarn.lock,.yarn-integrity,.yarn-metadata.json}"
          ],
          asar: true,
          appId: 'com.example.app',
          productName: 'My Electron App',
          nsis: {
            oneClick: false,
            allowToChangeInstallationDirectory: true,
          },
          mac: {
            target: ['zip'],
            identity: null
          },
          linux: {
            target: ['zip'],
            category: 'Utility',
            icon: './build/icon.png'
          },
          win: {
            target: ['zip'],
            icon: './build/icon.png'
          }
        }
      })
    }
  }
}