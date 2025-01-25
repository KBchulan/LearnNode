// 这一节我们说一下类型守卫

// 类型收缩
// 如下面这个可以筛选出any类型里的string类型，但是一些数组对象函数是不行的
const isString = (value: any): boolean => typeof value === 'string'
// 因此有instanceof
const isArr = (arr: any): boolean => arr instanceof Array


const isObj = (arg: any) => Object.prototype.toString.call(arg) === '[object Object]'
const isNum = (num: any) => typeof num === 'number'
const isStr = (str: any) => typeof str === 'string'
const isFunc = (func: any) => typeof func === 'function'
const fn = (data: any) => {
    if (isObj(data)) {
        let val;
        Object.keys(data).forEach(key => {
            val = data[key]
            if (isNum(val)) {
                data[key] = val.toFixed(2)
            }
            if (isStr(val)) {
                data[key] = val.trim()
            }
            if (isFunc(val)) {
                val()
            }
        })
    }
}

let obj = {
    name: 'huaixi',
    age: 19,
    fn: () => {
        console.log('fn')
    }
}
fn(obj)