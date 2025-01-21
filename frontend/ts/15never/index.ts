// 本节我们介绍一下never这种类型(bottom type)

// never是永远都不会发生值的类型，如下面这个
type A = string & number

const hx = (): never => {
    throw new Error('huaixi')
}

// 比如说只有这三个，我们有一个兜底的error，可以想一下B的联合类型再多几个会出现什么
// 没错，非bottom类型的元素赋值给never，那么就会报错
type B = '唱' | '跳' | 'rap'
function kun(value: B) {
    switch (value) {
        case '唱':
            console.log('唱')
            break
        case '跳':
            console.log('跳')
            break
        case 'rap':
            console.log('rap')
            break
        default:
            const error: never = value
            break
    }
}