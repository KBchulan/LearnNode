let map: Map<number, string> = new Map()
map.set(1, 'aaa')
map.set(2, 'bbb')
map.set(3, 'ccc')

for(let [key, value] of map){
  console.log(key, value);
}