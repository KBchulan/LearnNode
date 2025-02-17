class Animail {
  public sex: string

  constructor(sex: string) {
    this.sex = sex
  }

  print() {
    console.log(this.sex)
  }
}

interface Dogg {
  name: string
  battle: () => void
}

class Dog extends Animail implements Dogg {
  private readonly sound: string
  public readonly name: string

  constructor(sex: string, soung: string, name: string) {
    super(sex)
    this.sound = soung
    this.name = name
  }

  battle() {
    
  }
}