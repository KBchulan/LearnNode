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