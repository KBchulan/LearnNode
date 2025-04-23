
// 这里写一下Promise：Promise的本质是一个函数返回的对象，
// 我们可以在它上面绑定回调函数，这样我们就不需要在一开始把回调函数作为参数传入这个函数了
const p: Promise<string>/* 这个类型为result为true的返回类型(resolve的结果) */ = new Promise((resolve, reject) => {
  // 一些异步的操作
  let success: string = 'success' // 假设这个是操作成功的结果
  let fail: string = 'fail' // 假设这个是操作失败的结果
  const result: boolean = true

  if (result) {
    resolve(success)
  }
  else {
    reject(fail)
  }
})

// 如果异步操作成功，则调用then方法，但是如果异步操作失败或者then方法中抛出错误，则调用catch方法
// 这里的result是resolve的返回值，同样，error是reject的返回值
p.then((result) => {
  console.log(result)
}).catch((error) => {
  console.log(error)
})

