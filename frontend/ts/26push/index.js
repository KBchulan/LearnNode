"use strict";
// 本节我们来写一下发布订阅模式
class Emitter {
    constructor() {
        this.events = new Map();
    }
    once(event, callback) {
        const fn = (...args) => {
            callback(...args);
            this.off(event, fn);
        };
        this.on(event, fn);
    }
    on(event, callback) {
        var _a;
        if (this.events.has(event)) {
            (_a = this.events.get(event)) === null || _a === void 0 ? void 0 : _a.push(callback);
        }
        else {
            this.events.set(event, [callback]);
        }
    }
    emit(event, ...args) {
        const callbacks = (this.events.get(event));
        callbacks.forEach(fn => fn(...args));
    }
    off(event, callback) {
        const callbacks = (this.events.get(event));
        this.events.set(event, callbacks.filter(fn => fn !== callback));
    }
}
const bus = new Emitter();
const fn = (b, n) => {
    console.log(1, b, n);
};
const fn2 = (b, n) => {
    console.log(2, b, n);
};
const fn3 = (b, n) => {
    console.log(3, b, n);
};
const fn4 = (b, n) => {
    console.log(4, b, n);
};
// 订阅监听器
// bus.on('message', fn)
// bus.on('message', fn2)
// bus.on('message', fn3)
bus.once('message', fn4);
bus.off('message', fn);
console.log(bus);
// 派发监听器
bus.emit('message', false, 1);
bus.emit('message', false, 1);
bus.emit('message', false, 1);
bus.emit('message', false, 1);
