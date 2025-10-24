# import.meta 属性介绍

import.meta 是 ES 模块中的一个元数据对象，包含当前模块的信息。主要属性如下：

## 标准属性

* import.meta.url: 返回当前模块的完整 URL

## Vite 特有属性

* import.meta.env: 环境变量
  * import.meta.env.MODE: 应用运行的模式
  * import.meta.env.BASE_URL: 部署的基本 URL
  * import.meta.env.PROD: 是否为生产环境
  * import.meta.env.DEV: 是否为开发环境
  * import.meta.env.SSR: 是否为服务器端渲染

* import.meta.glob: 批量导入文件

```typescript
  const modules = import.meta.glob('./dir/*.js')
```

* import.meta.hot: HMR (热模块替换) API

## Node.js 特有属性

* import.meta.resolve: 解析模块说明符

## 其他环境

* Webpack: import.meta.webpack, import.meta.webpackHot
* Rollup: import.meta.rollupVersion
