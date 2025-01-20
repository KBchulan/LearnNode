// 本章我们介绍一下枚举类型
// 与c++对应一下，语法基本一致
// 但是ts会生成具体对象的
// 包括数字枚举，字符串枚举，异构枚举（两者混用）
var Color;
(function (Color) {
    Color[Color["Red"] = 3] = "Red";
    Color[Color["Green"] = 4] = "Green";
    Color[Color["Blue"] = 5] = "Blue";
    Color["Error"] = "black"; // c++中不可以
})(Color || (Color = {}));
console.log(Color[4]); // 通过value访问key，这和c++十分不同
console.log(Color.Green); // 通过key访问value
console.log(Color.Error);
var obj = {
    red: 3 // 不能为3以外的值(Color.Red可以)
};
var code = 0;
if (code === 0 /* Types.success */) {
}
