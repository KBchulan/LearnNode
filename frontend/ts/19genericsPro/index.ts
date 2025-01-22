// 泛型约束是很有必要的事情
// 如下面这段代码，若是Type为undefined呢，很显然需要约束
// const add = <Type>(a: TypeError, b: Type): Type => {
// return a + b
// } 

// 因此我们可以升级一下，用extends做约束
function add<Type extends number>(a: Type, b: Type): number {
    return a + b
}

console.log(add(1, 2))