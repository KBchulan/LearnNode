## Map 的迭代器详解

## 1. Map 的默认迭代器 (Symbol.iterator)

在 Map 中，`Symbol.iterator` 实际上等同于 `entries()`。这意味着 Map 的默认迭代行为是遍历键值对：

```typescript
const map = new Map([
    ['a', 1],
    ['b', 2],
    ['c', 3]
]);

// 以下两种写法是等价的
for (const entry of map) {
    console.log(entry); // ['a', 1], ['b', 2], ['c', 3]
}

for (const entry of map.entries()) {
    console.log(entry); // ['a', 1], ['b', 2], ['c', 3]
}
```

## 2. Map 提供的三种迭代器方法

Map 对象提供了三种迭代器方法：

### 2.1 entries()

返回键值对的迭代器：

```typescript
const map = new Map([['a', 1], ['b', 2]]);
const entriesIterator = map.entries();

console.log(entriesIterator.next().value); // ['a', 1]
console.log(entriesIterator.next().value); // ['b', 2]
```

### 2.2 keys()

返回键的迭代器：

```typescript
const map = new Map([['a', 1], ['b', 2]]);
const keysIterator = map.keys();

console.log(keysIterator.next().value); // 'a'
console.log(keysIterator.next().value); // 'b'
```

### 2.3 values()

返回值的迭代器：

```typescript
const map = new Map([['a', 1], ['b', 2]]);
const valuesIterator = map.values();

console.log(valuesIterator.next().value); // 1
console.log(valuesIterator.next().value); // 2
```

## 3. 迭代器的使用示例

### 3.1 使用 for...of 循环

```typescript
const map = new Map([['a', 1], ['b', 2], ['c', 3]]);

// 遍历键值对（默认）
for (const [key, value] of map) {
    console.log(`${key} => ${value}`);
}

// 只遍历键
for (const key of map.keys()) {
    console.log(key);
}

// 只遍历值
for (const value of map.values()) {
    console.log(value);
}
```

### 3.2 使用解构

```typescript
const map = new Map([['a', 1], ['b', 2]]);

// 解构键值对
const [...entries] = map;
console.log(entries); // [['a', 1], ['b', 2]]

// 解构键
const [...keys] = map.keys();
console.log(keys); // ['a', 'b']

// 解构值
const [...values] = map.values();
console.log(values); // [1, 2]
```

## 4. entries() 与 Symbol.iterator 的关系

```typescript
const map = new Map([['a', 1], ['b', 2]]);

// 证明 Symbol.iterator 返回的就是 entries() 迭代器
console.log(map[Symbol.iterator]() === map.entries()); // false（注意：它们是不同的迭代器实例）
console.log([...map[Symbol.iterator]()]); // [['a', 1], ['b', 2]]
console.log([...map.entries()]); // [['a', 1], ['b', 2]]
```

## 5. Map 迭代器的特点

1. **独立性**：每个迭代器都是独立的，有自己的迭代状态

```typescript
const map = new Map([['a', 1], ['b', 2]]);
const iter1 = map[Symbol.iterator]();
const iter2 = map[Symbol.iterator]();

console.log(iter1.next().value); // ['a', 1]
console.log(iter2.next().value); // ['a', 1]
```

2. **一次性**：迭代器使用完后就不能重用

```typescript
const map = new Map([['a', 1], ['b', 2]]);
const iterator = map.entries();

for (const entry of iterator) {
    console.log(entry); // 输出所有条目
}

// 迭代器已经用完
console.log([...iterator]); // []
```

3. **实时性**：迭代器反映 Map 的实时状态

```typescript
const map = new Map([['a', 1]]);
const iterator = map.entries();

map.set('b', 2); // 添加新元素
for (const entry of iterator) {
    console.log(entry); // 会输出包括新添加的元素
}
```

## 6. 最佳实践

1. 使用 for...of 时，直接使用 Map 而不是 entries()：

```typescript
// 推荐
for (const [key, value] of map) {
    // ...
}

// 不推荐
for (const [key, value] of map.entries()) {
    // ...
}
```

2. 需要多次遍历时，每次创建新的迭代器：

```typescript
// 正确做法
for (const entry of map) {
    // 第一次遍历
}
for (const entry of map) {
    // 第二次遍历
}
```
