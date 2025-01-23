// 默认导出：一个模块只能有一个默认导出(默认导出对象比较多)
export default (a: number, b: number): number => a + b

// 分别导出
export let x = 2
export const add = (a: number, b: number): number => a + b

// 解构导出
let a: number = 5
let arr: number[] = [1, 2, 3]
export {
    a,
    arr
}