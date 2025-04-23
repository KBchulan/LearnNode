async function func() {
  console.log('Y')
  await Promise.resolve()
  console.log('X')
}

setTimeout(() => {
  console.log(1)
  Promise.resolve().then(() => {
    console.log(2)
  })
})

setTimeout(() => {
  console.log(3)
  Promise.resolve().then(() => {
    console.log(4)
  })
})

Promise.resolve().then(() => {
  console.log(5)
})

Promise.resolve().then(() => {
  console.log(6)
})

Promise.resolve().then(() => {
  console.log(7)
})

func()
console.log(0)
