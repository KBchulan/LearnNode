interface Person {
    name: string
    age: number
    sex: string
}

let person: Person = {
    name: 'huaixi',
    age: 19,
    sex: 'man'
}

type Person2 = typeof person
