class Person {
  private _num: number

  constructor(num: number) {
    this._num = num
  }

  get num(){
    return this._num + 3
  }

  set num(num: number){
    this._num = num
  }
}

let obj: Person = new Person(2)
console.log(obj.num)
obj.num = 3