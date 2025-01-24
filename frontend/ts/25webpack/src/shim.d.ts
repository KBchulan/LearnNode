// 这是一个垫片
declare module "*.vue" {
    import { DefineComponent } from "vue"

    // 组件实例类型
    const component: DefineComponent<
        {}, // props 类型
        {}, // data 返回值类型
        any, // methods 类型
    >

    export default component
}