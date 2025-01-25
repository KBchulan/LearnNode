// 数组类型介绍定义以及获取

const ArrayTest = () => {
    // 通过基本数据类型来定义，下面为原始数据类型
    let arr1: number[] = [1, 2, 3]
    let arr2: string[] = ['a', 'b', 'c']
    let arr3: boolean[] = [true, false]
    let arr4: null[] = [null, null]
    let arr5: undefined[] = [undefined, undefined]
    let arr6: symbol[] = [Symbol(), Symbol()]   // 也可以加上descripe（string or number）
    let arr7: bigint[] = [2n ** 1024n, 9000000000000000000n ** 1024n]

    // 泛型约束，跟上面的一样
    let a1: Array<number> = [1, 2, 3]

    // 对象数组
    interface Person {
        age: number
    }
    let a2: Person[] = [{ age: 10 }, { age: 9 }]

    // 高维数组
    let a3: number[][] = [[1], [2], [3]]
    let a4: Array<Array<number>> = [[1], [2], [3]]

    // 函数
    let a5 = (...args: number[]) => { console.log(args) }
    a5(1, 2, 3)

    // 此处讲一下arguments的使用
    // arguments其实是一个包含所有函数参数的数组，但又不完全是一个数组
    function a6(...args: any[]) {
        console.log(arguments)         // [Arguments] { '0': 1, '1': true }
        // let a: any[] = arguments    // 这是错误的，因为arguments不是一个数组

        let a: IArguments = arguments  // 我们可以使用IArguments内置对象来接这个

        for (let index = 0; index < a.length; index++) {
            const element = a[index];
            console.log(element)
        }
    }
    a6(1, true)
}

// 这里简单介绍一下forEach
// 1. 数组的 forEach
const arr = [1, 2, 3];
arr.forEach((item, index, array) => {
    console.log(item);   // 当前元素
    console.log(index);  // 当前索引
    console.log(array);  // 原数组
});


// 2. Map 的 forEach
const map = new Map([
    ['a', 1],
    ['b', 2]
]);
map.forEach((value, key, map) => {
    console.log(value);  // 值
    console.log(key);    // 键
    console.log(map);    // 原 Map
});


// 3. Set 的 forEach
const set = new Set([1, 2, 3]);
set.forEach((value, key, set) => {
    console.log(value);  // 当前值
    console.log(key);    // 同值（Set 中值就是键）
    console.log(set);    // 原 Set
});
