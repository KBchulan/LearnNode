const fib = (n: number, meno: Map<number, number> = new Map()): number => {
  if (n <= 1)
    return n

  if (meno.has(n))
    return meno.get(n)!

  const result = fib(n - 1, meno) + fib(n - 2, meno)
  meno.set(n, result)
  return result
}

const fibRe = (() => {
  const cache = new Map<number, number>()

  return function func(n: number): number {
    if (n <= 1)
      return n

    if (cache.has(n))
      return cache.get(n)!

    const result = func(n - 1) + func(n - 2)
    cache.set(n, result)
    return result
  }
})()

const func = ((a: number) =>
              (b: number) =>
              (c: number) =>
              (d: number) => { })(1)(2)(3)(4)

console.log(fibRe(5))

