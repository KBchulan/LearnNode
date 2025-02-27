interface A {
  name: string
  age: number
}

interface B {
  name: string
  age: number
  gender: string
}

let a: A = {
  name: 'a',
  age: 1,
}

let b: B = {
  name: 'b',
  age: 2,
  gender: 'male',
}

a = b

console.log(a)
