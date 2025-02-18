interface Father {
  name: string
  age: number
}

interface Son {
  name: string
  age: number
  sex: string
}

let father: Father = {
  name: 'i am father',
  age: 40
}

let son: Son = {
  name: 'i am father',
  age: 20,
  sex: 'man'
}

let funcFather = (fa: Father) => { }
let funcSon = (son: Son) => { }

funcSon = funcFather