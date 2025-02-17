// 本章我们会介绍这个联合类型，交叉类型，类型断言

// 联合类型就是对同一个东西支持多种类型
let phone: number | string = 18737519552
phone = '+86-18737519552'

// 输入为数字或者bool类型都会返回实际的布尔类型，比如后端返回的东西里
// 'true'和1都希望表示true，我们可以通过两次取!来实现这个操作
let fn = (type: number | boolean): boolean => {
  return !!type
}

// 交叉类型类似于extends或者重合效果
// 毕竟基础类型交叉没有意义，我们的应用场景：
/**
 * 比如数据库配置，比如基础类型(host + port)我们写了一个interface，后续数据库单独写一个
 * 配置，两者MIXINS后可以作为一个供前端使用的统一配置
 * 交叉类型使用场景非常多，但本质来说就是将两个类型合并，然后返回一个新类型
 */
interface People {
  name: string,
  age: number
}

interface Man {
  sex: string
}

// 交叉类型是一个&，表示把所有的属性结合起来
const fn2 = (huaixi: Man & People): void => {
  console.log(huaixi)
}

fn2({
  name: 'huaixi',
  age: 19,
  sex: 'man'
})

// 类型断言:即借助as指定类型，或者说是起别名
// 此时若是传入string就会调用length方法，若是number就会返回一个undefined
// 说明这个as只是帮助我们通过这个编译，但是实际的运行时错误无法避免
let fn3 = (num: number | string): void => {
  console.log((num as string).length)
  console.log((<string>num).length)   // 不推荐，毕竟这种写法大家不常用
}
// 类型断言本质上只是为了通过编译，实际上没有任何转换，下面的例子，输入什么就输出什么
let fn4 = (type: any): boolean => {
  return (type as boolean)
}

// 这里加上一些操作符
namespace Operation {
  // 1.非空断言(!)，这个只是为了去除一些null或者undefined一类的属性
  export const func1 = (num: string | null): number => {
    return num!.length
  }

  // 2.可选链运算符(?.)
  export const obj1 = {
    first: {
      second: {
        finally: 123
      }
    }
  }
  // console.log(Operation.obj1?.first?.second?.finally) // 这样是正常访问
  // 但是如果过程中间有名字写错了，或者其他未知错误(比如null或者undefined)，
  // 是不会直接抛出异常的，而是返回undefined

  // 3.空值合并运算符(??)
  // m ?? n如果m不为空就返回m，否则返回n
  let n: number = 11
  export const num1: number = n ?? 15

  // 4.数字字面量(_)
  // 下面两种写法是等价的，只是方便人看
  export const num2: number = 123456789
  export const num3: number = 123_456_789
}
