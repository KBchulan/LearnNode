// interface 用于定义一个对象必须有什么属性和方法

// 最基础的演示，interface类似于一个契约，不能多属性，也不能少属性
const interface1 = () => {
    interface Person {
        name: string
        age: number
        live: boolean
        print: () => void
    }

    let a: Person = {
        name: "珂莱塔",
        age: 16,
        live: true,
        print: () => {
            console.log(`姓名: ${a.name}`)
            console.log(`年龄: ${a.age}`)
            console.log(`存活: ${a.live}`)
        }
    }

    a.print()
}

// 若是两个interface重名，则会进行一个重合的操作，即对象应当具有所有定义的属性
let interface2 = () => {
    interface Person {
        name: string
    }

    interface Person {
        age: number
    }

    let person: Person = {
        name: '菲比',
        age: 16
    }

    console.log(`姓名: ${person.name}`)
    console.log(`年龄: ${person.age}`)
}

// 调用接口返回一堆数据，但是我们只关注其中某几个，可以用索引签名的方式实现
let interface3 = () => {
    interface Person {
        // 前面两个属性是强校验的
        name: string
        age: number
        [elseElement: string]: any      // 可以对应任意个key，但是不关心，可以增加任意个
    }

    let person: Person = {
        name: '菲比',
        age: 16,
        a: 1,
        b: 2
    }

    console.log(`姓名: ${person.name}`)
    console.log(`年龄: ${person.age}`)
}

// ？与readonly
let interface4 = () => {
    interface Person {
        name: string,
        age?: number,                       // 表示可选，有没有都ok的
        readonly callback: () => boolean    // 加上readonly，则这个东西就不能更改了
    }

    let person: Person = {
        name: '菲比',   // 此处我们不加age也是可以的
        callback: () => {
            return false
        }
    }

    person.callback()   // ok
    // person.callback = () => { return true } // error
}

// 接口的继承：extends（类似于重合）
function interface5() {
    // 继承所有的属性和方法
    interface Person extends Person2, Person3 {
        name: string
    }

    interface Person2 {
        age: number
    }

    interface Person3 {
        love: string
    }

    let person: Person = {
        name: 'aaa',
        age: 100,
        love: 'bbb'
    }
}

// 定义函数类型
function interface6() {
    interface Func {
        (name: string): number[]
    }

    const func: Func = () => {
        return [1, 2]
    }

    func('')
}
