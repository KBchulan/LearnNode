import type { App } from 'vue'

// MutationObserver可以监听元素的属性变化，如宽高，位置等
// 懒加载图片时我们也用到了intersectionObserver
// 此处我们选择ResizeObserver来监听元素大小变化
function useResize(el: HTMLElement, callback: Function) {
  let resize = new ResizeObserver((entries) => {
    callback(entries[0].contentRect)  // contentRect代表被监听元素的尺寸和位置，{x,y,width,height,top,left,bottom,right}
  })
  resize.observe(el)
}

// 因为.use()方法会自动调用install方法，所以我们可以直接在install方法中注册指令
const install = (app: App) => {
  app.directive('resize', {
    mounted(el, binding) {
      useResize(el, binding.value)
    }
  })
}

useResize.install = install

export default useResize