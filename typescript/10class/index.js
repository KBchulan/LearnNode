"use strict";
// 本节介绍一下类
class Dom {
    // 创建节点
    createElement(el) {
        return document.createElement(el);
    }
    // 设置文本
    setText(el, text) {
        el.textContent = text;
    }
    // 渲染函数
    render(data) {
        let root = this.createElement(data.tag);
        if (data.children && Array.isArray(data.children)) {
            data.children.forEach(item => {
                let child = this.render(item);
                root.appendChild(child);
            });
        }
        else {
            this.setText(root, (data.text));
        }
        return root;
    }
}
class Vue extends Dom {
    // 构造函数
    constructor(options) {
        super(); // 初始化父类，写在子类前面
        this.options = options;
        this.init();
    }
    static version() {
        return 'v1.01';
    }
    init() {
        // 虚拟dom就是通过js去渲染我们这个真实的dom
        let data = {
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
        };
        let app = (typeof this.options.el == 'string' ?
            document.querySelector(this.options.el) : this.options.el);
        app.appendChild(this.render(data));
    }
}
console.log(Vue.version);
new Vue({
    el: "#app"
});
