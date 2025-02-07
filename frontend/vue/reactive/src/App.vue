<template>
    <div>
        <h2>reactive</h2>
        <hr>

        <input type="text" v-model="formRef"><br>
        <span>{{ formRef }}</span>
        <hr>

        <form>
            <input type="text" v-model="formReactive.name" class="form-item">
            <br>
            <input type="text" v-model="formReactive.age" class="form-item">
            <br>
            <button @click.prevent="submit" class="form-item">提交</button> <!-- 此处的prevent是阻止默认行为 -->
        </form>
        <hr>

        <ul>
            <li v-for="item in formReactiveArray" :key="item.name">
                <span>{{ item.name }}</span>
                <span>{{ item.age }}</span>
            </li>
            <button @click="add" class="add-btn">添加</button>
        </ul>
        <hr>

        <div>
            <span>{{ ObjShallowReactive }}</span><br>
            <button @click="edit">修改</button>
        </div>

    </div>

</template>

<script setup lang="ts">
// 简单说一下ref和reactive的区别，ref是响应式对象，reactive是响应式对象的代理
// 1. ref支持所有类型，但是reactive只支持引用类型
import { ref, reactive, readonly, shallowReactive } from 'vue'
let formRef = ref<string>('sakura')     // 如我们上一章所说，这里简单回顾一下ref的用法，这样子是可以的
// 但是reactive不行，reactive只能接收一个引用类型: reactive<T extends object>(target: T)

// 下面先说一下reactive的类型定义
type Form = {
    name: string,
    age: number
}
let formReactive = reactive<Form>({   // formReactive: Form也可以
    name: 'sakura',
    age: 18
})
// 2.ref取值和赋值需要.value，reactive不需要

// 上述也是reactive的应用场景，比如表单提交，我们就可以使用reactive来实现，下面再写一下数组的应用
// 我们知道，reactive是proxy的，所以不能直接覆盖，需要使用对应方法（如数组的push方法），如果直接覆盖，会破坏响应式
let formReactiveArray = reactive<Form[]>([
    {
        name: 'sakura',
        age: 18
    }
])


const submit = () => {
    console.log(formReactive)
}

const add = () => {
    setTimeout(() => {
        formReactiveArray.push({
            name: 'lumingfei',
            age: 19
        })
    }, 1000)
}

// reactive里也有readonly方法，可以创建一个只读的响应式对象，一眼知道什么意思，不说了

// 最后说一下shallowReactive，shallowReactive是浅层的响应式，只代理对象的第一层，不会代理对象的深层 
let ObjShallowReactive = shallowReactive({
    foo: {
        bar: {
            num: 1
        }
    }
})

const edit = () => {
    ObjShallowReactive.foo = { bar: { num: 2 } }
    console.log(ObjShallowReactive)
}

</script>

<style scoped>
.form-item {
    margin-bottom: 10px;
    margin-top: 10px;
    margin-left: 10px;
    margin-right: 10px;
    text-align: center;
}

.add-btn {
    margin-left: 10px;
    margin-right: 10px;
    text-align: center;
    color: yellow;
}
</style>
