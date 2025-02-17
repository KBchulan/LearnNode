interface Person {
  name: string
  age: number
  sex: string
}

// 这里有没有写:Person，下文类型是不一样的
/*
 * TypeScript 的类型推导会尽可能具体
 * 显式类型注解会覆盖类型推导
 * keyof 操作符总是返回属性名的联合类型
 * typeof 的结果取决于变量是否有类型注解
 */
let person: Person = {
  name: 'huaixi',
  age: 19,
  sex: 'man'
}

type Person2 = typeof person
type Person3 = keyof typeof person

const fn = (...args: number[]): void => {
  let arr: number[] = [1, 2, 3]
  console.log(args === arr)   // false，因为===比较的是地址，我们要比较内容
  console.log(args.toString === arr.toString)
  console.log(JSON.stringify(arr) === JSON.stringify(args))
}

function fn2(...args: number[]): void {
  let arr: IArguments = arguments
  /**
   * IArguments本质特性：
    1.是类数组对象，不是真正的数组
     *有自己的特殊属性（callee, length）
     *可以通过索引访问元素
     *没有数组的原型方法（map, filter 等）
    2.与数组的区别：
     *不继承自 Array.prototype
     *不能直接使用数组方法
     *有特殊的 callee 属性
     *length 属性是实时的
    3.最佳实践：
     *推荐使用剩余参数 (...args) 代替 arguments
     *如果必须使用 arguments，建议先转换为数组
     *在 TypeScript 中使用泛型来提供更好的类型安全
   */
  console.log(arr)
}

const arr: number[] = [1, 2, 3, 4, 5]
arr.forEach(value => console.log(value))

// 这里写一下Promise：Promise的本质是一个函数返回对象，
// 我们可以在它上面绑定回调函数，这样我们就不需要在一开始把回调函数作为参数传入这个函数了
const p: Promise<string>/* 这个类型为result为true的返回类型(resolve的结果) */ = new Promise((resolve, reject) => {
  // 一些异步的操作
  let success: string = 'success' // 假设这个是操作成功的结果
  let fail: string = 'fail' // 假设这个是操作失败的结果
  const result: boolean = true

  if (result) {
    resolve(success)
  }
  else {
    reject(fail)
  }
})

// 如果异步操作成功，则调用then方法，但是如果异步操作失败或者then方法中抛出错误，则调用catch方法
// 这里的result是resolve的返回值，同样，error是reject的返回值
p.then((result) => {
  console.log(result)
}).catch((error) => {
  console.log(error)
})