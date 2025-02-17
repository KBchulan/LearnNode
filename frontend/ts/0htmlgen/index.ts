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
   * 本质特性：
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