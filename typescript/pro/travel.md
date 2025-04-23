# JavaScript/TypeScript 数组遍历方法详解

## 1. 基础遍历方法

### for 循环

最传统的遍历方式，可以完全控制遍历过程：

```typescript
const arr = [1, 2, 3, 4, 5];
for (let i = 0; i < arr.length; i++) {
    console.log(arr[i]);
}
```

### for...of 循环

ES6引入的简洁遍历方式，直接获取元素值：

```typescript
const arr = [1, 2, 3, 4, 5];
for (const value of arr) {
    console.log(value);
}
```

### for...in 循环

遍历数组索引，但只是key值（不推荐用于数组）：

```typescript
const arr = [1, 2, 3, 4, 5];
for (const index in arr) {
    console.log(arr[index]);
}
```

## 2. 数组内置方法

### forEach 方法

为每个元素执行回调函数：

```typescript
const arr = [1, 2, 3, 4, 5];
arr.forEach((value, index, array) => {
    console.log(`值: ${value}, 索引: ${index}`);
});
```

### map 方法

创建一个新数组，其结果是对每个元素调用函数处理：

```typescript
const arr = [1, 2, 3, 4, 5];
const doubled = arr.map(value => value * 2);
console.log(doubled); // [2, 4, 6, 8, 10]
```

### filter 方法

创建一个新数组，包含通过测试的元素：

```typescript
const arr = [1, 2, 3, 4, 5];
const evenNumbers = arr.filter(value => value % 2 === 0);
console.log(evenNumbers); // [2, 4]
```

### reduce 方法

将数组元素计算为一个值：

```typescript
const arr = [1, 2, 3, 4, 5];
const sum = arr.reduce((acc, curr) => acc + curr, 0);
console.log(sum); // 15
```

## 3. 迭代器方法

### entries()

同时获取索引和值：

```typescript
const arr = [1, 2, 3, 4, 5];
for (const [index, value] of arr.entries()) {
    console.log(`索引: ${index}, 值: ${value}`);
}
```

### keys() 和 values()

分别获取索引迭代器和值迭代器：

```typescript
const arr = [1, 2, 3, 4, 5];

// 遍历索引
for (const index of arr.keys()) {
    console.log(index);
}

// 遍历值
for (const value of arr.values()) {
    console.log(value);
}
```

## 4. 查找和测试方法

### find() 方法

返回第一个满足条件的元素：

```typescript
const arr = [1, 2, 3, 4, 5];
const found = arr.find(value => value > 3);
console.log(found); // 4
```

### some() 方法

测试是否至少有一个元素满足条件：

```typescript
const arr = [1, 2, 3, 4, 5];
const hasEven = arr.some(value => value % 2 === 0);
console.log(hasEven); // true
```

### every() 方法

测试是否所有元素都满足条件：

```typescript
const arr = [1, 2, 3, 4, 5];
const allPositive = arr.every(value => value > 0);
console.log(allPositive); // true
```

## 注意事项

1. `forEach` 不能使用 break 和 continue，也不能提前返回
2. `for...in` 主要用于对象遍历，用于数组可能会有问题
3. `map`、`filter`、`reduce` 等方法会创建新数组，原数组不变
4. `for...of` 是推荐的数组遍历方式，可以使用 break/continue
5. 对于大数组，传统的 for 循环性能最好

## 选择建议

- 简单遍历：使用 `for...of`
- 需要索引：使用传统 `for` 循环或 `forEach`
- 需要转换元素：使用 `map`
- 需要筛选元素：使用 `filter`
- 需要汇总数据：使用 `reduce`
- 需要查找元素：使用 `find`、`some`、`every`
