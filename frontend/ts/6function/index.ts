// 这一节介绍一下函数

// 指定参数和返回值类型
function add(...args: number[]): number {
    return args.reduce((sum, current) => sum + current, 0)
}

const add2 = (...args: number[]): number => args.reduce((sum, current) => sum + current, 0)

// 默认参数和可选参数
// 默认参数和c++的一致，从右往左入
// 可选参数就是加上一个?，和interface一样
function add3(a: number = 3, b: number = 5): number {
    return a + b
}

function add3else(a: number = 3, b?: number): number {
    return a + (b ?? 0)     // 空值合并运算符，如果b为null或undefined返回0,其他返回具体的值
}

// 参数为对象时
interface Person {
    name: string
    age: number
}

function add4(person1: Person = { name: 'a', age: 5 },
    person2: Person = { name: 'b', age: 6 }): number {
    return person1.age + person2.age
}

// this类型
// 若是类内：
// 1.默认指向类的实例
// 2.可以在类方法参数中声明 this 的类型
// 3.箭头函数会保持定义时的 this 上下文

// 作参数时
// 1.必须是第一个参数
// 2.仅用于类型检查，运行时不会传入
interface Obj {
    user: Array<number>,
    add: (this: Obj, num: number) => void
}

let obj: Obj = {
    user: [1, 2, 3],
    add(num) {
        this.user.push(num)
    },
}

// 函数重载
let user: number[] = [1, 2, 3]

function findNum(): number[];                   // 查询所有的
function findNum(id: number): number[];         // 查询自己
function findNum(nums: number[]): number[];     // 增加一个数组

function findNum(ids?: number | number[]): number[] {
    if (typeof ids == 'number') {
        return user.filter(v => v == ids)
    }
    else if (Array.isArray(ids)) {
        user.push(...ids)
        return user
    }
    else {
        return user
    }
}