// 本节我们会来学习一下泛型工具
// Partial: 所有属性（可选）
// Required：所有属性（必选）
// Pick：提取部分属性
// Exclude：排除部分属性
// Omit：排除部分属性， 并且返回剩余的属性
// Record：约束对象的Key和Value
// ReturnType：获取函数返回值的类型

interface User {
    address: string
    name: string
    age: number
}

type PartialUser = Partial<User>
type RequiredUser = Required<PartialUser>
type PickUser = Pick<User, 'address' | 'name'>
type ExcludeUser = Exclude<keyof User, 'age'>
type OmitUser = Omit<User, 'age'>

type Key = 'address' | 'name'
type Value = string | number
type RecordUser = Record<Key, Value>

type Func = () => string
type ReturnTypeUser = ReturnType<Func>

// 以下是自定义实现
type CustomPartial<T> = {
    [key in keyof T]?: T[key]
}
type CustomPartialUser = CustomPartial<User>

type CustomRequired<T> = {
    [key in keyof T]-?: T[key]
}
type CustomRequiredUser = CustomRequired<PartialUser>

type CustomPick<T, K extends keyof T> = {
    [key in K]: T[key]
}
type CustomPickUser = CustomPick<User, 'address' | 'name'>

type CustomExclude<T, K> = T extends K ? never : T  // never 在联合类型中会被忽略
type CustomExcludeUser = CustomExclude<keyof User, 'age'>

type CustomOmit<T, K extends keyof T> = Pick<T, Exclude<keyof T, K>>
type CustomOmitUser = CustomOmit<User, 'age'>

type CustomRecord<K extends keyof any, V> = {
    [key in K]: V
}
type CustomRecordUser = CustomRecord<Key, Value>

type CustomReturnType<T extends (...args: any) => any> = T extends (...args: any) => infer R ? R : never
type CustomReturnTypeUser = CustomReturnType<Func>