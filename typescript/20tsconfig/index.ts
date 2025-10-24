// 本节我们主要介绍 tsconfig.json文件，直接写注释了

function* gen() {
  var index: number = 0
  while (true) {
    yield index++
  }
}

let gens = gen()
console.log(gens.next().value)
console.log(gens.next().value)
console.log(gens.next().value)
console.log(gens.next().value)
