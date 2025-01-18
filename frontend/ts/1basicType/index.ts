// ts包含所有js基础类型
// basicType: boolean, number, string, null, undefined

// 初始化
// tsc --init

// 监听
// tsc --watch index.ts
// 或 tsc -w index.ts

// 运行
// node index.js

let declarationsTest = () => {
    // var作用于整个函数，因此两个x输出都为2
    function varTest() {
        var x = 1
        if (true) {
            var x = 2
            console.log(x)
        }
        console.log(x)
    }

    // let作用于自己的层级，因此输出2 1
    // const也是这样
    let letTest = () => {
        let x = 1
        if (true) {
            let x = 2
            console.log(x)
        }
        console.log(x)
    }
    // 因此，var允许重复声明，所以我们应该优先使用const和let

    varTest()
    letTest()
}

declarationsTest()
