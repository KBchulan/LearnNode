type BusClass = {
  emit: (name: string) => void
  on: (name: string, callback: Function) => void
}

type PramsKey = number | string | symbol

type List = {
  [key: PramsKey]: Array<Function>
}

class Bus implements BusClass {
  list: List
  constructor() {
    this.list = {}
  }

  emit(name: string, ...args: Array<any>) {
    let events: Array<Function> = this.list[name]
    events.forEach(value => {
      value.apply(this, args)
    })
  }

  on(name: string, callback: Function) {
    let fn: Array<Function> = this.list[name] || []
    fn.push(callback)
    this.list[name] = fn
  }
}

export default new Bus()