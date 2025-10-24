// 本节我们学一下协变，逆变，双向协变

// 协变：子类型可以赋值给父类型（更多的属性可以赋值给更少的属性）
// 逆变：父类型可以赋值给子类型（在函数参数类型中发生）
// 双向协变：父类型和子类型可以互相赋值

// 在ts中，函数参数默认是双向协变的
// 但是，在ts中，我们可以通过设置strictFunctionTypes来改变这个行为
// 设置为true，则函数参数变为逆变（更严格，更安全）
// 设置为false，则保持双向协变（更灵活，但不太安全）

// main
interface A {
  name: string
  age: number
}

// son
interface B {
  name: string
  age: number
  sex: string
}

let a: A = {
  name: 'huaixi',
  age: 19
}

let b: B = {
  name: 'aaa',
  age: 19,
  sex: 'man'
}

a = b           // 协变：子类型 B 可以赋值给父类型 A
console.log(a)

let funcA = (params: A) => { }
let funcB = (params: B) => { }

funcB = funcA   // 函数发生的是逆变，即父类给子类