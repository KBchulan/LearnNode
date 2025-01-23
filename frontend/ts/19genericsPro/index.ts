// 泛型约束是很有必要的事情
// 如下面这段代码，若是Type为undefined呢，很显然需要约束
// const add = <Type>(a: TypeError, b: Type): Type => {
// return a + b
// } 

// 因此我们可以升级一下，用extends做约束
function add<Type extends number>(a: Type, b: Type) {
    return a + b
}
console.log(add(1, 2))

// 还有一种情况，你想传入的参数比如都有length()方法，c++中，你可以通过concepts或SFINAE来进行约束
// 此时ts怎么做呢，我们可以借助interface进行约束
interface Len {
    length: number
}
function len<T extends Len>(a: T) {
    return a.length
}
console.log(len('aaaa'))
console.log(len([1, 2, 3, 4]))

// 对对象的约束
// 我们需要引入keyof这个关键字：用于获取对象类型的所有键的联合类型
let obj = {
    name: "huaixi",
    sex: "man"
}
type Key = keyof typeof obj

function func<T extends object, U extends keyof T>(obj: T, key: U) {
    return obj[key]
}
console.log(func(obj, 'name'))

// 演示一下keyof的用法，比如如下声明，我想给每个属性都变成可选的
interface Person {
    name: string
    age: number
    sex: string
}

type Options<T extends object> = {
    readonly [Key in keyof T]?: T[Key]
}
type Res = Options<Person>