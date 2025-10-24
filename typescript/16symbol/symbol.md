# TypeScript 中的 `symbol` 类型

## 简介

`symbol` 是 ECMAScript 6 (ES6) 新增的一种原始数据类型。它是一种唯一且不可变的数据类型，通常用作对象属性的键。`symbol` 类型在 TypeScript 中也得到了支持。

## 创建 `symbol`

可以使用 `Symbol` 函数来创建一个 `symbol`：

```typescript
let sym1 = Symbol();
let sym2 = Symbol("description");
```

每次调用 `Symbol` 函数都会创建一个唯一的 `symbol`，即使它们的描述相同：

```typescript
let sym1 = Symbol("key");
let sym2 = Symbol("key");
console.log(sym1 === sym2); // false
```

## 用作对象属性键

通常使用 `symbol` 来定义对象的属性键，以确保这些键是唯一的：

```typescript
let sym = Symbol();

let obj = {
    [sym]: "value"
};

console.log(obj[sym]); // "value"
```

## 全局 `symbol`

可以使用 `Symbol.for` 和 `Symbol.keyFor` 方法来创建和访问全局 `symbol`。全局 `symbol` 是在全局 symbol 注册表中注册的，可以在不同的模块或作用域中共享：

```typescript
let globalSym1 = Symbol.for("globalKey");
let globalSym2 = Symbol.for("globalKey");

console.log(globalSym1 === globalSym2); // true

let key = Symbol.keyFor(globalSym1);
console.log(key); // "globalKey"
```

## 内置 `symbol`

JavaScript 还提供了一些内置的 `symbol`，它们可以用来改变语言内部的行为。这些内置的 `symbol` 包括：

- `Symbol.iterator`: 定义对象的默认迭代器。
- `Symbol.asyncIterator`: 定义对象的默认异步迭代器。
- `Symbol.hasInstance`: 用于 `instanceof` 操作符。
- `Symbol.isConcatSpreadable`: 控制数组和类似数组对象是否可以展开。
- `Symbol.match`: 用于字符串的 `match` 方法。
- `Symbol.replace`: 用于字符串的 `replace` 方法。
- `Symbol.search`: 用于字符串的 `search` 方法。
- `Symbol.split`: 用于字符串的 `split` 方法。
- `Symbol.toPrimitive`: 用于将对象转换为原始值。
- `Symbol.toStringTag`: 用于对象的默认字符串描述。
- `Symbol.unscopables`: 定义不可用在 `with` 语句中的属性名称。

示例：

```typescript
class MyClass {
    [Symbol.toStringTag]: string = "MyClass";
}

let myInstance = new MyClass();
console.log(Object.prototype.toString.call(myInstance)); // "[object MyClass]"
```

## 总结

`symbol` 类型为我们提供了一种创建唯一标识符的方式，并且它们可以用作对象的属性键以避免属性名冲突。同时，内置的 `symbol` 还允许我们自定义和控制一些语言内部的行为。在 TypeScript 中使用 `symbol` 类型与在 JavaScript 中的用法基本一致，只是多了一些类型检查的保障。
