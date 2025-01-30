// 本节我们介绍一下泛型

import { rejects } from "assert"
import { resolve } from "path"

// 为什么要引入泛型，作者默认你的c++标准起码到17的水准，不再赘述，此处只是介绍一下ts
// 里的泛型怎么写

const huaixi = <Type>(a: Type, b: Type): Array<Type> => {
    return [a, b]
}

console.log(huaixi(1, 2))
console.log(huaixi<number>(1, 2))

// 这里也可以看出，ts是只存在值传递的，没有引用传递的概念
function swap<Type>(a: Type, b: Type): void {
    [a, b] = [b, a]
}

// 除此之外，type，interface都可以使用泛型
type A<Type> = string | number | Type
let a: A<boolean> = 'aaa'
a = 123
a = true

// interface
interface Person<Type, Upe> {
    msg: Type
    data: Upe
}

let person: Person<string, number> = {
    msg: 'aaa',
    data: 123
}

// 常见的多泛和默认泛型都是老生常谈了
function add<T = number, U = number>(a: T, b: U) {
    return [a, b]
}

// 接下来写一些例子
const axios = {
    get<T>(url: string): Promise<T> {
        return new Promise((resolve, reject) => {
            let xhr: XMLHttpRequest = new XMLHttpRequest()
            xhr.open('GET', url)
            xhr.onreadystatechange = () => {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    resolve(JSON.parse(xhr.responseText))
                }
            }
            xhr.send(null)
        })
    }
}

interface Data {
    msg: string
    code: number
}

axios.get<Data>('').then(res => {
    console.log(res)
})