// npm init -y 快速初始化项目(-y是--yes的缩写，确认了许多东西)
// npm i @types/node -D 是引入nodejs里的ts类型定义包，-D则保证只会影响开发环境包
// 其实只需要ts --init 就可以用 ts-node index.ts 直接运行了

import { on } from "events"

// 本文件主要介绍 any 和 unknown 两种顶级类型
/*
类型共有6个级别，从上到下依次是：
  1. any，unknown
  2. Object
  3. Number, String, Boolean
  4. number, string, boolean, null, undefined
  5. 4对应的实例字面量
  6. never
  
  原始类型：string, number, boolean, null, undefined, void, never, symbol, bigint, any, unknown
  此外引用类型包括：Array, Object, Function, Tuple, enum, Map, Set, WeekMap, WeekSet
*/

let anyOrunknown = () => {
  // any和unknown都是可以对应所有类型的
  let a: any = Symbol("huaixi")       // 可以自行了解一下Symbol
  a = 100n
  a = 14
  a = 'aaa'
  a = true
  a = {}
  a = []
  a = () => { }

  // 区别在于
  // 1.unknown只能赋值给unknown和any类型
  // 2.unknown无法读任何属性，也无法调用任何方法
  // 3.unknown更安全
  // eg: 下面这个any才可以使用
  let obj: any = {
    珂莱塔: 2.0,
    cout: () => { console.log('cute') }
  }
  console.log(obj.珂莱塔)
  obj.cout()
}

anyOrunknown()