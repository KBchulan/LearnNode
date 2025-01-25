// 本节我们来介绍一下Map,Set,WeakMap,WeakSet

// 首先是set
let set: Set<number> = new Set([9, 1, 2, 3, 4, 5, 6, 6, 6])
// 天然去重，引用类型除外
console.log(set)    // Set(7) { 9, 1, 2, 3, 4, 5, 6 }
// 常用方法
console.log(set.add(7))     // 加到末尾
console.log(set.has(7))     // 是否存在某一个数值
set.delete(6)               // 去除某一个元素
set.clear()                 // 清空

// 常见的遍历也都是支持的
// set.forEach((value, key, set) => {
//     console.log(value, key, set)
// })

// for (let value of set) {
//     console.log(value)
// }

// 接着是Map
// map的key可以是引用类型
let obj2 = {
    name: 'huaixi'
}
let map: Map<object, any> = new Map()
map.set(obj2, 'hhh')     // 其他方法点一下基本都知道是干什么的就ok

// weakmap，weakset：弱引用
// weakmap的key只能是引用类型
const weakMap = new WeakMap();
let obj = { id: 1 };
weakMap.set(obj, 'data');    // 键必须是对象
weakMap.get(obj);
weakMap.has(obj);
weakMap.delete(obj);
// 不能遍历，没有size


// 4. WeakSet: 弱引用对象集合
const weakSet = new WeakSet();
weakSet.add(obj);           // 只能添加对象
weakSet.has(obj);
weakSet.delete(obj);

// 总之weak修饰作为弱引用类型，不会对引用类型数据增加引用计数，因此不会记入垃圾回收
