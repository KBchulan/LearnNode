import type { App, VNode } from "vue"
import Loading from './Loading.vue'
import { createVNode, render } from 'vue'

// 插件导出一个对象格式，对象中需要有install方法
export default {
  install(app: App) {
    const vnode: VNode = createVNode(Loading)
    render(vnode, document.body)
    // 本质上还是增加全局属性给ts代码调用，但是我们想要获取到组件的这些方法
    // 所以需要使用defineExpose暴露组件的属性和方法，这是通过ts获取vue的实例
    app.config.globalProperties.$loading = {
      show: vnode.component?.exposed?.show,
      hide: vnode.component?.exposed?.hide
    }
  }
}