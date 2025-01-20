// 本节是类型推导和类型别名

// 就像这种ts非常简单就能推导出来是string，我们就没必要显示的写出来类型
let str = 'huaixi'

// 若是不显示写出来类型，则会推导为any
let num
num = 123; num = 'aaa'

// 类型别名，和c语言的typedef比较像，但是c的typedef只能给现有类型起别名，TS的type可以创建新的类型结构
type s = string | number | (() => void)     // 任意类型都可以，联合，交叉什么都可以
let str2: s = 'bbb'

// 与interface很像吧，都是定义类型的契约，区别是什么呢，type不具备重合继承等特性，除此
// 以外基本没有区别

// type高级用法
type num2 = 1 extends number ? 1 : 0 // extends在type是包含的意思