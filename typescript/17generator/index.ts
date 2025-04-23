// 本节我们介绍一下生成器和迭代器

// 生成器
function* gen() {
  yield 'huaixi'  // 同步
  yield Promise.resolve('hauixi') //异步
}

const xi = gen()
console.log(xi.next())  // { value: 'huaixi', done: false }
console.log(xi.next().value)      // huaixi
console.log(xi.next())  // { value: Promise { 'hauixi' }, done: false }
// 若是后面的done为true，表示没有东西可供迭代了

// yieldType, returnTtpe, nextType
function* dialogue(): Generator<string, void, string> {
  const response1 = yield '你好吗?'
  console.log(response1)
}
const dia = dialogue()
console.log(dia.next().value)

// 迭代器
// 迭代器的本质是一个具有 next() 方法的对象，它维护一个内部指针（或状态），
// 通过 next() 方法依次返回序列中的每个元素，并跟踪遍历的进度，直到遍历完成。

// 此处是set(非重复集合，与c++不同的是这个没有自动升序)
let set: Set<number> = new Set([1, 2, 6, 2, 1])
console.log(set)

// map的key比对象更为宽泛，它甚至可以使用array等引用类型作为参数
let map: Map<number, string> = new Map()
map.set(1, 'aaa')
let iterator = map[Symbol.iterator]()
let it = map.entries() // 与上一行等价
console.log(iterator.next().value)

// 此外还有许多如arguments,querySelectorAll等伪数组，那有没有一种统一的格式支持所有的遍历
// 有的兄弟，有的，就是我们的iterator
const each = (value: any) => {
  let it: any = value[Symbol.iterator]()
  let next: any = { done: false }
  while (!next.done) {
    next = it.next()
    if (!next.done) {
      console.log(next.value)
    }
  }
}
each(map)

// 迭代器的语法糖
// 但是for of不能使用对象的遍历
for (let value of map) {
  console.log(value)
}

// 这里说一下for of和for in的区别
// for...of：遍历值（可迭代对象的值）
// for...in：遍历键（对象的所有可枚举属性）
// 总结就是for in遍历key，for of是所有的值（若有key则是key与value的集合）

// 此处再说一下解构，其实和c++17的结构化绑定是一样的
let [a, b, c] = [1, 2, 3]
console.log(a, b, c)
// 这是很简单的用法，但实际上不管是这种简单的结构还是参数包的底层都是调用iterator的

let obj = {
  max: 5,
  current: 0,
  [Symbol.iterator]() {
    return {
      max: this.max,
      current: this.current,
      next() {
        if (this.current === this.max) {
          return {
            value: undefined,
            done: true
          }
        }
        else {
          return {
            value: this.current++,
            done: false
          }
        }
      }
    }
  }
}

for (let value of obj) {
  console.log(value)
}

// 后话：
// interface Generator<T = unknown, TReturn = any, TNext = unknown> extends Iterator<T, TReturn, TNext> {
//     // NOTE: 'next' is defined using a tuple to ensure we report the correct assignability errors in all places.
//     next(...args: [] | [TNext]): IteratorResult<T, TReturn>;
//     return(value: TReturn): IteratorResult<T, TReturn>;
//     throw(e: any): IteratorResult<T, TReturn>;
//     [Symbol.iterator](): Generator<T, TReturn, TNext>;
// }
// 其实生成器就是迭代器的一个子实现