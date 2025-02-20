### 为什么要有虚拟DOM

  DOM的本质是通过js生成的AST抽象节点树，或者可以说是真实DOM的javascript对象。

```typescript
<!-- 这就是vnode的结构 -->
interface VNode {
  type: string | Component;     // 节点类型
  props: Record<string, any>;   // 属性
  children: VNode[] | string;   // 子节点
}

<!-- 真实 DOM -->
<div class="user">
  <span>张三</span>
</div>

<!-- 对应的 VNode（简化表示） -->
{
  type: 'div',
  props: { class: 'user' },
  children: [
    {
      type: 'span',
      props: {},
      children: '张三'
    }
  ]
}
```

  在vue里面就是把vue语法转换为用js描述的AST，注意此时的js生成的是我们vue所指定的这些elements，并非所有的，是因为我们知道一个真实DOM其实是有非常多属性的，那么包含所有属性的方式是比较愚蠢的，那么如果我们需要的属性被单独描述为树呢，是不是在性能上就比较良好了，也就是我们虚拟DOM的由来。

### Diff算法

  请参考src/APP.vue的内容
