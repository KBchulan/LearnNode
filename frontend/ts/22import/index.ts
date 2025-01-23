// 默认导出的话import后面的名字随便起
import xxx from './test'
console.log(xxx)

// 分别导出：用一个{}，里面写对应的名字，可以与默认导出之间用‘，’分割开
import { x, add } from './test'
console.log(x)
console.log(add(1, 2))

// 解构导出（对象传递）
import { a, arr as arr2 } from './test'
console.log(a, arr2)

// 所有的
import * as api from './test'
console.log(api)

// 动态引入(Promise对象)
if (true) {
    import('./test').then(res => {
        console.log(res)
    })
}