// ts包含所有js基础类型
// basicType: boolean, number, string, null, undefined

// 初始化
// tsc --init

// 编译
// tsc index.ts

// 监听
// tsc --watch index.ts
// 或 tsc -w index.ts

// 运行
// node index.js

let declarationsTest = () => {
  // var作用于整个函数，因此两个x输出都为2
  function varTest() {
    var x = 1
    if (true) {
      var x = 2
      console.log(x)
    }
    console.log(x)
  }

  // let作用于自己的层级，因此输出2 1
  // const也是这样
  let letTest = () => {
    let x = 1
    if (true) {
      let x = 2
      console.log(x)
    }
    console.log(x)
  }
  // 因此，var允许重复声明，所以我们应该优先使用const和let
  // 特别推荐是，函数参数使用const，函数体使用let，for循环使用let

  varTest()
  letTest()
}

let dataTypeTest = () => {
  let str: string = 'huaixi'              // 普通字符串(单双引号都可以)
  let num: number = 123                   // 普通数字
  let templateStr: string = `str:${num}`  // 模板字符串，类似python的f字符串

  // 特殊的
  let num1: number = NaN          // not a number
  let num2: number = Infinity     // 无穷大
  let num3: number = 0b111        // 二进制
  let num4: number = 0o77         // 八进制
  let num5: number = 0xA56D       // 十六进制

  // 其他基础类型
  let b: boolean = true
  let n1: null = null
  let n2: undefined = undefined

  // void
  // 在严格模式下只能被undefined赋值,宽松模式下才可以是null(严格模式修改tsconfig.json)
  let v1: void = undefined
  // let v2: void = null

  // 同理，宽松模式下下面两行才能成立
  // n1 = n2
  // n2 = n1

  console.log(str)
  console.log(num)
  console.log(templateStr)
  console.log(num1)
  console.log(num2)
  console.log(num3)
  console.log(num4)
  console.log(num5)
  console.log(b)
  console.log(n1)
  console.log(n2)
}

declarationsTest()
dataTypeTest()
