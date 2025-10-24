// 下面写一下call方法
// 语法为func.call(thisArg, ...args)，即第一个参数会被认为是这个函数的this对象
// 剩下的则是参数
interface Num {
  num: number
}
let num: Num = { num: 3 }
class Person {
  name: string
  static num: number = 6
  constructor(_name: string, _num: number) {
    this.name = _name
    Person.num = _num
  }
  static add(a: number, b: number): number {
    return this.num + a + b
  }
}

console.log(Person.add.call(num, 3, 5));  // 若第一个参数为null/undefined，则此处的this表示Person.num，但是传入了对象后，this就变了
