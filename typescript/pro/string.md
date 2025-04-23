# JavaScript/TypeScript String Methods Reference
*Last updated: 2025-02-21 08:52:48 UTC by KBchulan*

## 1. 字符串访问方法
| 方法/属性 | 描述 | 示例 |
|---------|------|------|
| `length` | 返回字符串的长度 | `"hello".length // 5` |
| `charAt(index)` | 返回指定位置的字符 | `"hello".charAt(0) // "h"` |
| `charCodeAt(index)` | 返回指定位置字符的 Unicode 值 | `"A".charCodeAt(0) // 65` |
| `at(index)` | 返回指定位置的字符（支持负数索引） | `"hello".at(-1) // "o"` |

## 2. 搜索方法
| 方法 | 描述 | 示例 |
|-----|------|------|
| `indexOf(searchValue[, fromIndex])` | 查找子串第一次出现的位置 | `"hello".indexOf('l') // 2` |
| `lastIndexOf(searchValue[, fromIndex])` | 查找子串最后一次出现的位置 | `"hello".lastIndexOf('l') // 3` |
| `includes(searchString[, position])` | 判断是否包含指定字符串 | `"hello".includes('he') // true` |
| `startsWith(searchString[, position])` | 判断是否以指定字符串开头 | `"hello".startsWith('he') // true` |
| `endsWith(searchString[, length])` | 判断是否以指定字符串结尾 | `"hello".endsWith('lo') // true` |

## 3. 截取和分割方法
| 方法 | 描述 | 示例 |
|-----|------|------|
| `slice(beginIndex[, endIndex])` | 提取字符串的一部分 | `"hello".slice(0, 2) // "he"` |
| `substring(indexStart[, indexEnd])` | 提取字符串的一部分 | `"hello".substring(1, 4) // "ell"` |
| `split([separator[, limit]])` | 将字符串分割成数组 | `"h,e,l,l,o".split(',') // ["h","e","l","l","o"]` |

## 4. 转换方法
| 方法 | 描述 | 示例 |
|-----|------|------|
| `toLowerCase()` | 转换为小写 | `"Hello".toLowerCase() // "hello"` |
| `toUpperCase()` | 转换为大写 | `"hello".toUpperCase() // "HELLO"` |
| `trim()` | 删除两端空白 | `" hello ".trim() // "hello"` |
| `trimStart()` | 删除开头空白 | `" hello".trimStart() // "hello"` |
| `trimEnd()` | 删除结尾空白 | `"hello ".trimEnd() // "hello"` |

## 5. 替换和填充方法
| 方法 | 描述 | 示例 |
|-----|------|------|
| `replace(searchFor, replaceWith)` | 替换第一个匹配项 | `"hello".replace('l', 'L') // "heLlo"` |
| `replaceAll(searchFor, replaceWith)` | 替换所有匹配项 | `"hello".replaceAll('l', 'L') // "heLLo"` |
| `padStart(targetLength[, padString])` | 在开头填充 | `"5".padStart(3, "0") // "005"` |
| `padEnd(targetLength[, padString])` | 在结尾填充 | `"5".padEnd(3, "0") // "500"` |
| `repeat(count)` | 重复字符串 | `"ha".repeat(3) // "hahaha"` |

## 6. 正则表达式方法
| 方法 | 描述 | 示例 |
|-----|------|------|
| `match(regexp)` | 返回匹配结果 | `"hello".match(/l/g) // ["l","l"]` |
| `matchAll(regexp)` | 返回所有匹配的迭代器 | `"hello".matchAll(/l/g) // Iterator` |
| `search(regexp)` | 返回首个匹配的索引 | `"hello".search(/l/) // 2` |

## 7. 实用示例

```typescript
// 1. 基础字符串操作
const str = "Hello, World!";
console.log(str.length);          // 13
console.log(str.toUpperCase());   // "HELLO, WORLD!"
console.log(str.toLowerCase());   // "hello, world!"

// 2. 字符串查找
console.log(str.includes("World")); // true
console.log(str.startsWith("Hello")); // true
console.log(str.indexOf("o"));    // 4

// 3. 字符串修改
console.log(str.replace("Hello", "Hi")); // "Hi, World!"
console.log("  space  ".trim());  // "space"
console.log("123".padStart(5, "0")); // "00123"

// 4. 字符串分割
console.log(str.split(", "));     // ["Hello", "World!"]
console.log(str.slice(0, 5));     // "Hello"

// 5. 字符串模式匹配
console.log("hello world".match(/o/g));  // ["o", "o"]
```

## 8. 注意事项

1. **不可变性**
   - 所有字符串方法都不会修改原字符串
   - 总是返回新的字符串

2. **索引**
   - 字符串索引从 0 开始
   - 负数索引在大多数方法中不支持（除了 `at()`）

3. **性能考虑**
   - 频繁的字符串拼接应使用数组或 `StringBuilder`
   - 大量替换操作考虑使用正则表达式

4. **兼容性**
   - `replaceAll()` 是较新的方法，可能需要 polyfill
   - `at()` 方法在某些旧版本浏览器中不支持

5. **Unicode**
   - 处理 Unicode 字符时要特别注意
   - 考虑使用 `normalize()` 方法处理 Unicode 规范化