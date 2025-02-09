export namespace AAA {
    export let aaa = 4
}

const Func = (): Function => {
    let num: number = 3
    const func = (): void => {
        console.log(num)
    }

    return func
}

const func = Func()
func()