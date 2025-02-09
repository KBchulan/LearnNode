// 本章我们会介绍这个联合类型，交叉类型，类型断言

// 联合类型就是对同一个东西支持多种类型
let phone: number | string = 18737519552
phone = '+86-18737519552'

// 输入为数字或者bool类型都会返回实际的布尔类型
let fn = (type: number | boolean): boolean => {
    return !!type
}

// 交叉类型类似于extends或者重合效果
interface People {
    name: string,
    age: number
}

interface Man {
    sex: string
}

// 交叉类型是一个&，表示把所有的属性结合起来
const fn2 = (huaixi: Man & People): void => {
    console.log(huaixi)
}

fn2({
    name: 'huaixi',
    age: 19,
    sex: 'man'
})

// 类型断言:即借助as指定类型，或者说是起别名
// 此时若是传入string就会调用length方法，若是number就会返回一个undefined
// 说明这个as只是帮助我们通过这个编译，但是实际的运行时错误无法避免
let fn3 = (num: number | string): void => {
    console.log((num as string).length)
    console.log((<string>num).length)   // 不推荐，毕竟这种写法不好
}
// 类型断言本质上只是为了通过编译，实际上没有任何转换，下面的例子，输入什么就输出什么
let fn4 = (type: any): boolean => {
    return (type as boolean)
}
