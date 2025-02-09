// 本章我们介绍一下枚举类型

// 与c++对应一下，语法基本一致
// 但是ts会生成具体对象的

// 包括数字枚举，字符串枚举，异构枚举（两者混用）
enum Color {
    Red = 3,    // 自动递增
    Green,
    Blue,
    Error = 'black' // c++中不可以
}

// 反向映射
console.log(Color[4])       // 通过value访问key，这和c++十分不同
console.log(Color.Green)    // 通过key访问value
console.log(Color.Error)  

// 接口枚举
interface Obj {
    red: Color.Red
}

let obj: Obj = {
    red: 3  // 不能为3以外的值(Color.Red可以)
}

// const枚举
const enum Types {
    success,
    fail
}

let code: number = 0
if (code === Types.success) {

}
/*
  普通枚举：生成实际的枚举对象，支持反向映射
  const枚举：直接内联值，不生成对象，节省运行时开销
*/
