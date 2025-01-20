// 本节介绍一下元组

// 本质上是一个多类型的大集合体

let arr1: [number, string] = [123, 'aaa']    // 固定长度元组，每个位置类型固定，是不能越界的

const arr2: [number, string] = [123, 'aaa']
// arr2 = []    // 错误
arr2[0] = 23    // 但是具体元素可以修改

const arr3: readonly [x: number, y?: string] = [123]    //这样就都不能改了

// 实例，如excel的二维表格
const excel: [string, string, number][] = [
    ['huaixi', 'man', 19],
    ['huaixi', 'man', 19],
    ['huaixi', 'man', 19],
    ['huaixi', 'man', 19]
]

let num: number = 123
console.log(typeof num)

type first = typeof arr1[0]
type length = typeof arr1['length']
console.log(arr1['length'])