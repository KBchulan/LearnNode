# JS执行机制

&ensp;&ensp;我们都知道js是单线程的，因为如果在同一时间同时对同一个DOM进行操作，js就不知道该执行哪个操作，所以js是单线程的，但是在h5以后，js也支持了多线程，也就是我们的web worker，但是web worker不能操作DOM，也不能操作window，所以js还是单线程的。

&ensp;&ensp;单线程就意味着所有的任务都需要排队，后面的任务需要等前面的任务执行完才能执行，如果前面的任务耗时过长，后面的任务就需要一直等，一些从用户角度上不需要等待的任务就会一直等待，这个从体验角度上来讲是不可接受的，所以JS中就出现了异步的概念。

## 任务分类

### 任务(Task)
- script(整体代码)
- setTimeout
- setInterval
- UI交互事件
- postMessage
- Ajax

### 微任务(MicroTask)
- Promise.then catch finally
- MutaionObserver
- process.nextTick(Node.js 环境)

## 事件循环

&ensp;&ensp;JavaScript使用一个事件循环模型来处理异步任务。事件循环包含以下关键组件：

### 任务队列(Task Queue)
* 执行诸如setTimeout、setInterval、I/O操作、UI渲染事件等
* 包括整体脚本执行(script)、DOM操作、用户事件、网络请求等
* 一次事件循环只处理一个任务

### 微任务队列(MicroTask Queue)
* 处理Promise回调(.then/.catch/.finally)、queueMicrotask()、MutationObserver等
* 微任务在当前任务结束后立即执行
* 所有微任务会在下一个任务开始前清空队列

## 执行组件

### 调用栈（Call Stack）
&ensp;&ensp;JavaScript引擎维护一个调用栈，用于跟踪代码执行的位置。所有函数调用都会形成一个执行上下文（Execution Context）并压入栈中。

### 堆（Heap）
&ensp;&ensp;对象分配的内存区域，负责动态内存分配。

### 事件循环（Event Loop）
&ensp;&ensp;负责协调调用栈、任务队列和微任务队列的执行顺序。

### Web API/Node API
&ensp;&ensp;宿主环境提供的API，如DOM、setTimeout、network等，提供异步功能支持。

## 详细执行顺序

### 初始化阶段
1. 全局执行上下文创建：
   - 创建全局对象（浏览器中为window，Node.js中为global）
   - 建立this绑定（指向全局对象）
   - 创建变量环境和词法环境
   - 变量和函数声明被提升（Hoisting）

### 执行阶段
1. 同步代码执行：
   - 同步代码按顺序入栈并执行
   - 函数调用创建新的执行上下文并压入栈顶
   - 执行完毕的上下文从栈顶弹出
2. 异步任务处理：
   - 遇到异步API（如setTimeout）时，回调被注册到Web API/Node API
   - 异步操作完成后，回调被推入相应队列
     - 任务回调 → 任务队列（Task Queue）
     - 微任务回调 → 微任务队列（Microtask Queue）

### 事件循环具体步骤
1. 检查调用栈是否为空
2. 若不为空，继续执行栈内代码
3. 调用栈为空时：
   - 处理所有微任务：
     - 遍历整个微任务队列
     - 将每个微任务移入调用栈执行
     - 执行过程中可能产生新的微任务，将继续处理直到队列清空
     - 微任务间没有UI渲染
   - 执行一个任务：
     - 从任务队列取出最早的一个任务
     - 创建新的执行上下文并入栈
     - 执行该任务，可能产生新的微任务或注册新的任务
   - 再次处理所有微任务队列
     - 确保任务执行过程中产生的所有微任务都被执行完毕
   - 渲染阶段（仅浏览器环境）：
     - 计算样式（Style Calculation）
     - 布局计算（Layout）
     - 绘制（Paint）
     - 合成（Compositing）
     - 注意：渲染不一定在每轮循环都发生，通常遵循屏幕刷新率（60Hz）
     - requestAnimationFrame回调在渲染前执行
4. 重复循环：
   - 返回步骤1，开始新一轮循环

## 底层排队机制

### 微任务队列细节
- 实际上每个执行上下文都有自己的微任务队列
- Promise的实现使用微任务队列，这是规范要求的
- 微任务执行顺序是FIFO（先进先出）
- queueMicrotask() API可直接将回调加入微任务队列

### 任务队列细节
- 浏览器实际维护多个任务队列，分不同优先级：
  - 用户交互任务（如点击）- 高优先级
  - 网络和I/O任务 - 中优先级
  - setTimeout/setInterval - 低优先级
- 不同任务源（Task Source）的任务可能进入不同队列
- 浏览器可能根据启发式算法决定先处理哪个队列的任务

### 内存和性能考量
- 微任务队列过长可能导致任务队列饥饿（Task Starvation）
- 垃圾回收通常在任务之间进行，作为独立任务
- 长时间运行的JavaScript会阻塞UI更新

### 不同环境的差异
- 浏览器: 有UI渲染和DOM操作
- Node.js: 有特殊的nextTick队列，优先级高于Promise微任务

# 总结
1. 主进程有一个调用栈来执行同步代码，同时外界有一个任务队列来处理任务和微任务
2. 任务队列中又分为宏任务和微任务，微任务的优先级高于宏任务
3. 微任务会在当前任务结束后立即执行，宏任务会在当前任务结束后执行
