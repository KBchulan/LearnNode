// 本节我们介绍一下对象混入和类混入

// 对象混入
/*
    1.简单合并属性
    2.使用 Object.assign
    3.浅拷贝，后面覆盖前面
*/
const obj1 = {
    a: 1,
    b: 2
}
const obj2 = {
    b: 3,
    c: 4
}

// 这两种混入方式有什么区别呢，你可以看到obj3返回的是一个新对象类型，但是obj4则是一个交叉类型
const obj3 = { ...obj1, ...obj2 }               // es6的语法，扩展运算符，浅拷贝
const obj4 = Object.assign({}, obj1, obj2)      // es5的语法，浅拷贝
console.log(obj3)
console.log(obj4)

// 深拷贝
console.log(structuredClone(obj1))

// 类混入，说白了就是再写一个类，含有所有方法
// 插件类型的混入
class App {
    run() {
        console.log('run')
    }
}

class Logger {
    log(msg: string) {
        console.log(msg)
    }
}

class Html {
    render() {
        console.log('render')
    }
}

// 我现在想给这个app装上几个插件
type Custructor<T> = new (...args: any[]) => T
function pluginMinxins<T extends Custructor<App>>(Base: T) {
    return class extends Base {
        private Logger = new Logger()
        private Html = new Html()

        constructor(...args: any[]) {
            super(...args)
            this.Logger = new Logger()
            this.Html = new Html()
        }

        // 此时就能混入了
        run() {
            this.Logger.log('run2')
        }
    }
}

const mixins = pluginMinxins(App)
const app = new mixins()

app.run()