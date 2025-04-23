// 本节我们介绍一下抽象类

// 与c++中带有纯虚函数的类一个道理，不过ts有显示的语法
// abstruct 修饰类则为抽象类，无法实例化
// abstruct 修饰方法则是抽象方法，不能实现，且子类必须实现

abstract class Vue {
  name: string

  constructor(name?: string) {
    this.name = name!
  }

  getName(): string {
    return this.name
  }

  abstract abFunc(): void // 该方法不能实现
}

// let vue = new Vue() // 错误，抽象类不能实例化

// 派生类必须实现抽象方法，和纯虚函数一个道理
class React extends Vue {
  constructor() {
    super()
  }

  abFunc(): void {

  }

  setName(name: string): void {
    this.name = name
  }
}

let react: React = new React()
react.setName('huaixi')
console.log(react.getName())