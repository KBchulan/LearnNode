// 本节我们介绍一下namespace
// 作用的话依旧和c++是一样的，我们着重介绍用法
// 导出 嵌套 合并(和instance一样) 抽离(多文件) 简化(类似python的from module import ...，可以直接使用了就)
import { AAA } from "./test"

namespace Test {
    // 命名空间内部需要export导出才能使用
    export let test = 1
    export const add = (a: number, b: number): number => a + b

    export namespace Son {      // 嵌套
        export let son = 2
    }
}

namespace Test {            // 合并
    export let test2 = 3
}

console.log(Test.test)
console.log(Test.Son.son)
console.log(AAA.aaa)

// 这里不得不说一下闭包，先来看一下mdn的介绍：
// 闭包是由捆绑起来（封闭的）的函数和函数周围状态（词法环境）的引用组合而成。
// 换言之，闭包让函数能访问它的外部作用域。

// 一句话：闭包允许封闭函数访问其作用域之外的变量（哪怕该变量所在的作用域已经消失）
// mdn：https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Closures

// 接下来说一说本质
/*
    写了一个函数它内部有一套词法环境，
    又写一个封闭函数做引用指向堆内存，
    但是在存在引用模块时这个词法环境的闭包帧会维持下去
*/

/*
    所以说闭包的本质还可以这么说，是不同的模块共同维持一个堆区内存的访问，
    只要还存在访问，那这个内存就不会被释放，c++的伪闭包也是模拟堆区的这个操作
*/