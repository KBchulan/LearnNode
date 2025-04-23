// 本节来介绍一下Ts的内置对象

// ECMAScript内置
// 类型为对应new出来的类型
let num: Number = new Number(1)
let date: Date = new Date()
let reg: RegExp = new RegExp(/.*?/)
let error: Error = new Error('error')
let xhr: XMLHttpRequest = new XMLHttpRequest()

// DOM： 网页内容
// 类型为HTML(元素名称)Element, 但是若是section这种无特殊标签的类型实际上会修正为HTMLElement
// 下面有两个例子，一个是非空断言，一个是类型守卫
let element1: HTMLDivElement = document.querySelector('div')!
let element2 = document.querySelector('section')
if (element2) { }

let elements1: NodeList = document.querySelectorAll('div')
let elements2: NodeListOf<HTMLDivElement | HTMLCanvasElement> = document.querySelectorAll('div canvas')

// BOM：浏览器或者Window内容
let local: Storage = localStorage
let lo: Location = location
let promise: Promise<string> = new Promise((resolve, reject) => {
  resolve('successful')

  reject('failed')
})

promise.then(res => {
  console.log(res)
})

let cookie: string = document.cookie