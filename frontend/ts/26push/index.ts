// 本节我们来写一下发布订阅模式

// once on emit off 订阅中心Map<事件名称，Function[]>

interface I {
  events: Map<string, Function[]>                       // 事件中心
  once: (event: string, callback: Function) => void     // 触发一次订阅器
  on: (event: string, callback: Function) => void       // 订阅监听器
  emit: (event: string, ...args: any[]) => void         // 派发监听器
  off: (event: string, callback: Function) => void      // 删除监听器
}

class Emitter implements I {
  events: Map<string, Function[]>     // 同一事件的回调函数集合

  constructor() {
    this.events = new Map()
  }

  once(event: string, callback: Function) {
    const fn = (...args: any[]) => {
      callback(...args)
      this.off(event, fn)
    }
    this.on(event, fn)
  }

  on(event: string, callback: Function) {
    if (this.events.has(event)) {
      this.events.get(event)?.push(callback)
    } else {
      this.events.set(event, [callback])
    }
  }

  emit(event: string, ...args: any[]) {
    const callbacks = (this.events.get(event))!
    callbacks.forEach(fn => fn(...args))
  }

  off(event: string, callback: Function) {
    const callbacks = (this.events.get(event))!
    this.events.set(event, callbacks.filter(fn => fn !== callback))
  }
}

const bus = new Emitter()
const fn = (b: boolean, n: number) => {
  console.log(1, b, n)
}
const fn2 = (b: boolean, n: number) => {
  console.log(2, b, n)
}
const fn3 = (b: boolean, n: number) => {
  console.log(3, b, n)
}
const fn4 = (b: boolean, n: number) => {
  console.log(4, b, n)
}

// 订阅监听器
bus.on('message', fn)
bus.on('message', fn2)
bus.on('message', fn3)
bus.once('message', fn4)

bus.off('message', fn)

console.log(bus)

// 派发监听器
bus.emit('message', false, 1)
bus.emit('message', false, 1)
bus.emit('message', false, 1)
bus.emit('message', false, 1)
