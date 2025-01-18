// 本文档介绍Object, object, {}的区别

// 1. Object（大写）：所有类型的顶级类型
let ObjectTest = () => {
    let a1: Object = 123;        // 基本类型 ok
    let a2: Object = '123';      // 字符串 ok
    let a3: Object = true;       // 布尔值 ok
    let a4: Object = {};         // 对象 ok
    let a5: Object = [];         // 数组 ok
    let a6: Object = () => 213;  // 函数 ok

    // 但是不能访问具体方法
    // a1.toFixed();  // 错误：Object 类型上不存在属性 'toFixed'
}

// 2. object（小写）：所有非原始类型
// 如对象，数组，函数，还有Set，Map一类的，交叉类型和引用类型也可以，只要能引用即可
let objectTest = () => {
    // let a1: object = 123;     // 错误：基本类型不能赋值给 object
    // let a2: object = '123';   // 错误：字符串不能赋值给 object
    // let a3: object = true;    // 错误：布尔值不能赋值给 object

    let a4: object = {};         // ok
    let a5: object = [];         // ok
    let a6: object = () => 213;  // ok

    // 引用类型都可以
    let set: object = new Set([1, 2, 3]);
    let map: object = new Map([["name", true]]);
    let date: object = new Date();

    // 自定义类的实例也可以
    class Person {
        name: string = "张三";
    }
    let person: object = new Person();
}

// 3. {}：空对象类型，和 Object 类似，但有一些限制
let EmptyObjectTest = () => {
    let a1: {} = 123;        // ok
    let a2: {} = '123';      // ok
    let a3: {} = true;       // ok
    let a4: {} = {};         // ok
    let a5: {} = [];         // ok
    let a6: {} = () => 213;  // ok

    // 但是不能添加新属性
    let obj: {} = { name: "张三" };  // ok
    // obj.age = 20;  // 错误：类型 '{}' 上不存在属性 'age'

    // 也不能访问除了 Object 原型链上的方法之外的其他方法
    // obj.name;  // 错误：类型 '{}' 上不存在属性 'name'
}
