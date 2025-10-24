// electron 的主进程文件

import { app, BrowserWindow } from 'electron'

app.whenReady().then(() => {
  const window = new BrowserWindow({
    width: 500,
    height: 400,
    webPreferences: {
      nodeIntegration: true,  // 允许在渲染进程中使用Node.js API，这样可以在前端代码中直接使用Node.js的功能
      contextIsolation: false, // 禁用上下文隔离，使渲染进程可以访问主进程中的对象和API，但这会降低安全性
      webSecurity: false      // 关闭跨域检测
    }
  })

  const url = process.argv[2]
  if (url) {
    window.loadURL(url)
  }
  else {
    window.loadFile('index.html')
  }
})