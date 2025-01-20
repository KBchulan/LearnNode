// 本节介绍一下类

// 1.基本用法，继承和类型约束
// 2.修饰符号readonly, private, public, protected(后面这三个和c++的基本一致,默认级别不一样罢了)
// 3.super:指向父类的函数使用：父类的prototype.constructor.call
// 4.静态(与c++也是一样的)
// 5.get，set

// implements用于约束
// extends用于继承
interface Options {
    el: string | HTMLElement
}

interface VueClass {
    options: Options
    init(): void
}

interface Vnode {
    tag: string,
    text?: string,
    children?: Vnode[]
}

class Dom {
    // 创建节点
    createElement(el: string) {
        return document.createElement(el)
    }

    // 设置文本
    setText(el: HTMLElement, text: string | null) {
        el.textContent = text
    }

    // 渲染函数
    protected render(data: Vnode) {
        let root = this.createElement(data.tag)
        if (data.children && Array.isArray(data.children)) {
            data.children.forEach(item => {
                let child = this.render(item)
                root.appendChild(child)
            })
        } else {
            this.setText(root, (data.text) as string)
        }

        return root
    }
}

class Vue extends Dom implements VueClass { // extends必须写在implements前面
    readonly options: Options

    // 构造函数
    constructor(options: Options) {
        super()     // 初始化父类，写在子类前面
        this.options = options
        this.init()
    }

    static version(): string {
        return 'v1.01'
    }

    init(): void {
        // 虚拟dom就是通过js去渲染我们这个真实的dom
        let data: Vnode = {
            tag: "div",
            children: [
                {
                    tag: "section",
                    text: "1"
                },
                {
                    tag: "section",
                    text: "2",
                }
            ]
        }
        let app = (typeof this.options.el == 'string' ?
            document.querySelector(this.options.el) : this.options.el)!
        app.appendChild(this.render(data))
    }
}

console.log(Vue.version)
new Vue({
    el: "#app"
})

class Ref {
    _value: any
    constructor(value: any) {
        this._value = value
    }

    get value() {
        return this._value + 'bbb'
    }

    set value(value: any) {
        this._value = value + 'ccc'
    }
}

// get和set会做拦截操作，和c++不太一样
const ref: Ref = new Ref('aaa')
console.log(ref.value)
ref.value = ''
console.log(ref.value)