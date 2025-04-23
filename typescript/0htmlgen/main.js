// 这里我们先说一下原型链这个东西

/**
 * Prototype 原型 | 原型对象
 *  1. Prototype是{函数（也可以说是类）}的一个属性
 *  2. Prototype是一个对象，这个对象有很多方法，如：toString、apply、call等
 *  3. 当我们创建一个函数时，会默认加上Prototype这个属性
 * 
 * __proto__ 隐式原型
 *  1. {对象}的属性
 *  2. 指向上一级函数的prototype
 * 
 * 顶层：Object.prototype.__proto__ === null
 */

function fn() {
  return true
}
console.dir(fn)

const obj = new fn()
console.log(obj.__proto__ === fn.prototype)

// 那为什么被称之为链呢
// 我们可以想一下，函数的本质是一个对象，fn.prototype按照我们上文所说是一个对象，那对象是不是又有__proto__这个属性
console.log(fn.prototype.__proto__)
// 这个其实就是我们的Object的prototype
console.log(fn.prototype.__proto__ === Object.prototype)
// 那同样的，Object.prototype还可以往上，不过是空了，因为Object就是顶级类型了
// 那么我们就能明白了，通过prototype拿到对象，再对对象访问__proto__访问到上一级的prototype，也就成了我们的链
console.log(obj.__proto__.__proto__ === Object.prototype);

/**
 * 差不多就是这意思
 * obj = {
 *  a: 5,
 *  __proto__: fn.prototype = {
 *    b: 10,
 *    __proto__: Object.prototype = {
 *      __proto__: null
 *    }
 *  }
 * }
 */

// 那我们的查找就是
// 1. 先看obj对象本身有没有这个属性
// 2. 如果没有，就去__proto__中找
// 3. 如果还没有，就去Object.prototype中找
// 4. 如果还没有，就返回undefined
// 一句话，顺着链条去找
obj.a = 5
fn.prototype.b = 10
console.log(obj.a)  // 5
console.log(obj.b)  // 10