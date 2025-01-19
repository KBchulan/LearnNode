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

// 参数为对象时
interface Person {
    name: string,
    age: number
}

function add4(person1: Person = { name: 'a', age: 5 },
    person2: Person = { name: 'b', age: 6 }): number {
    return person1.age + person2.age
}

// this类型
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