import { Plugin } from 'postcss'

const Options = {
  viewportWidth: 375,
}

interface Options {
  viewportWidth: number
}

export const postcssPxToViewport = (options: Options = Options): Plugin => {
  const opt = { ...Options, ...options }
  return {
    postcssPlugin: 'postcss-px-to-viewport',
    // 很多钩子函数, 这里的Declaration可以获取到每个css的声明
    Declaration(node) {
      if (node.value.includes('px')) {
        const num = parseFloat(node.value)
        const vw = (num / opt.viewportWidth) * 100
        node.value = `${vw.toFixed(2)}vw`
      }
    }
  }
}