<script setup lang="ts">
import { ref, computed, reactive } from 'vue'
// 这里做一个样例，我们期望有两个响应式对象，一个姓一个名，分别用v-model绑定，最后计算出来全名显示
let firstName = ref<string>('张')
let lastName = ref<string>('三')

/**
 * 先说computed(计算属性)整体认知：
 * 1.缓存特性:
    - computed 会基于其【响应式依赖】进行缓存
    - 只有当依赖的数据发生变化时，才会重新计算
    - 多次访问 computed 属性时，如果依赖没有变化，会直接返回缓存的结果
   2.计算特性：
    - computed是基于其他数据派生出的数据
    - 主要应用：
      - 数据转换：需要基于现有数据计算出新的数据时
      - 数据过滤：需要过滤或排序数据时
      - 复杂计算：涉及多个数据源的复杂计算
      - 性能优化：避免在模板中放置太多逻辑计算
 */

// computed是有两个重载的，第一种是这种options式的写法，我们需要传入一个自定义get和set方法的对象
/**
 * export interface WritableComputedOptions<T, S = T> {
    get: ComputedGetter<T>;
    set: ComputedSetter<S>;
  }
 */
let name = computed<string>({
  get() {
    return firstName.value + '*' + lastName.value
  },
  set(newValue: string) {
    [firstName.value, lastName.value] = newValue.split('-')  // 把原来的名字分割一下
    console.log(newValue)
  }
})

const changeName = (): void => {
  name.value = '李-四'
}

// 函数式的写法，这个方法只有一个getter方法，不能赋值，因为对应
let nameFunc = computed(() => { return firstName.value + '-' + lastName.value })

// 接下来写一个样例
interface Data {
  name: string
  price: number
  num: number
}
let data = reactive<Data[]>([
  {
    name: '棒棒糖',
    price: 1,
    num: 3
  },
  {
    name: '巧克力',
    price: 2,
    num: 4
  },
  {
    name: '胡萝卜',
    price: 3,
    num: 111
  }
])

const getTotal = (): number => {
  let total: number = 0
  for (let item of search.value) {
    total += item.num * item.price
  }
  return total
}
// also do like this
let total = computed(() => search.value.reduce((total, cur) => {
  return total + cur.num * cur.price
}, 0))
let keyWord = ref<string>('')

const deleteFunc = (index: number) => {
  data.splice(index, 1)
}
const search = computed(() => {
  return data.filter((item: Data) => {
    return item.name.includes(keyWord.value)
  })
})

</script>

<template>
  <div>
    <div>
      姓: <input v-model="firstName" type="text">
    </div>
    <div>
      名: <input v-model="lastName" type="text">
    </div>
    <div>
      全名: {{ nameFunc }}
    </div>
    <div>
      <button @click="changeName">修改</button>
    </div>
    <hr>
    <div>
      <input v-model="keyWord" placeholder="搜索" type="text">
    </div>
    <div style="margin-top: 20px;">
      <table border width="600" cellpadding="0" cellspacing="0">
        <thead>
          <tr>
            <th>物品名称</th>
            <th>物品单价</th>
            <th>物品数量</th>
            <th>物品总价</th>
            <th>操作</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="(item, index) in search">
            <td align="center"> {{ item.name }} </td>
            <td align="center"> {{ item.price }} </td>
            <td align="center">
              <button @click="item.num > 0 ? item.num-- : item.num = 0">-</button>
              {{ item.num }}
              <button @click="item.num < 1000 ? item.num++ : item.num = 0">+</button>
            </td>
            <td align="center"> {{ item.price * item.num }} </td>
            <td align="center"> <button @click="deleteFunc(index)">删除</button> </td>
          </tr>
        </tbody>
        <tfoot>
          <tr>
            <td colspan="5" align="right">
              总价：{{ getTotal() }}--{{ total }}
            </td>
          </tr>
        </tfoot>
      </table>
    </div>
  </div>
</template>

<style scoped></style>
