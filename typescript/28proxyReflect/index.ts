// proxy的本质其实就是也是代理，和nginx里面的并无不同，不过这个更好的说法是es6里面的拦截器
const target = {
  name: 'huaixi',
  age: 19
}

const handler = {
  // 1. get：属性读取
  get(target: any, property: string, receiver: any) {
    console.log('get:', property);
    return Reflect.get(target, property, receiver);
  },

  // 2. set：属性设置
  set(target: any, property: string, value: any, receiver: any) {
    console.log('set:', property, value);
    return Reflect.set(target, property, value, receiver);
  },

  // 3. has：in 操作符
  has(target: any, property: string) {
    console.log('has:', property);
    return Reflect.has(target, property);
  },

  // 4. deleteProperty：delete 操作符
  deleteProperty(target: any, property: string) {
    console.log('delete:', property);
    return Reflect.deleteProperty(target, property);
  },

  // 5. apply：函数调用
  apply(target: any, thisArg: any, argumentsList: any[]) {
    console.log('apply:', argumentsList);
    return Reflect.apply(target, thisArg, argumentsList);
  },

  // 6. construct：new 操作符
  construct(target: any, args: any[], newTarget: any): object {
    console.log('construct:', args);
    return Reflect.construct(target, args, newTarget);
  },

  // 7. getOwnPropertyDescriptor：Object.getOwnPropertyDescriptor
  getOwnPropertyDescriptor(target: any, property: string) {
    console.log('getOwnPropertyDescriptor:', property);
    return Reflect.getOwnPropertyDescriptor(target, property);
  },

  // 8. defineProperty：Object.defineProperty
  defineProperty(target: any, property: string, descriptor: PropertyDescriptor) {
    console.log('defineProperty:', property);
    return Reflect.defineProperty(target, property, descriptor);
  },

  // 9. getPrototypeOf：Object.getPrototypeOf
  getPrototypeOf(target: any) {
    console.log('getPrototypeOf');
    return Reflect.getPrototypeOf(target);
  },

  // 10. setPrototypeOf：Object.setPrototypeOf
  setPrototypeOf(target: any, prototype: any) {
    console.log('setPrototypeOf');
    return Reflect.setPrototypeOf(target, prototype);
  },

  // 11. isExtensible：Object.isExtensible
  isExtensible(target: any) {
    console.log('isExtensible');
    return Reflect.isExtensible(target);
  },

  // 12. preventExtensions：Object.preventExtensions
  preventExtensions(target: any) {
    console.log('preventExtensions');
    return Reflect.preventExtensions(target);
  },

  // 13. ownKeys：Object.getOwnPropertyNames 和 Object.getOwnPropertySymbols
  ownKeys(target: any) {
    console.log('ownKeys');
    return Reflect.ownKeys(target);
  }
};

const proxy = new Proxy(target, handler)
proxy.name;                     // get
proxy.value = 456;              // set
'name' in proxy;                // has
delete proxy.name;              // deleteProperty
Object.getPrototypeOf(proxy);   // getPrototypeOf

// 其实本质是去拦截一些操作