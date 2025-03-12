# Flutter 常用 Widget 选择指南

## 1. 基础结构类 Widget

### Scaffold - 材料设计的基本页面结构

```dart
return Scaffold(
  appBar: AppBar(title: Text('标题')),
  body: Container(...),
  bottomNavigationBar: BottomNavigationBar(...),
);
```

### Container - 装饰和定位的容器

```dart
return Container(
  margin: EdgeInsets.all(8.0),
  padding: EdgeInsets.all(16.0),
  decoration: BoxDecoration(
    color: Colors.blue,
    borderRadius: BorderRadius.circular(8),
  ),
  child: Text('Hello'),
);
```

### Material - 材料设计的基础widget

```dart
return Material(
  elevation: 4.0,
  color: Colors.white,
  child: Center(child: Text('Hello')),
);
```

## 2. 布局类 Widget

### Row - 水平布局

```dart
return Row(
  mainAxisAlignment: MainAxisAlignment.center,
  children: [
    Icon(Icons.star),
    Text('评分'),
    Text('4.5'),
  ],
);
```

### Column - 垂直布局

```dart
return Column(
  mainAxisSize: MainAxisSize.min,
  children: [
    Image.asset('logo.png'),
    Text('标题'),
    Text('描述'),
  ],
);
```

### Stack - 层叠布局

```dart
return Stack(
  children: [
    Image.asset('background.jpg'),
    Positioned(
      bottom: 10,
      right: 10,
      child: Text('版权信息'),
    ),
  ],
);
```

### Wrap - 自动换行布局

```dart
return Wrap(
  spacing: 8.0,
  children: [
    Chip(label: Text('标签1')),
    Chip(label: Text('标签2')),
    Chip(label: Text('标签3')),
  ],
);
```

## 3. 定位和大小类 Widget

### Center - 居中布局

```dart
return Center(
  child: Text('居中文本'),
);
```

### Align - 自定义对齐

```dart
return Align(
  alignment: Alignment.topRight,
  child: Text('右上角'),
);
```

### Expanded - 填充剩余空间

```dart
return Row(
  children: [
    Text('固定宽度'),
    Expanded(
      child: Container(color: Colors.blue),
    ),
  ],
);
```

### SizedBox - 固定大小

```dart
return SizedBox(
  width: 100,
  height: 100,
  child: Card(...),
);
```

## 4. 滚动类 Widget

### ListView - 列表滚动

```dart
return ListView(
  children: [
    ListTile(title: Text('项目1')),
    ListTile(title: Text('项目2')),
  ],
);
```

### SingleChildScrollView - 单子组件滚动

```dart
return SingleChildScrollView(
  child: Column(
    children: [/* 长内容 */],
  ),
);
```

### GridView - 网格布局

```dart
return GridView.count(
  crossAxisCount: 2,
  children: [
    Card(child: Text('1')),
    Card(child: Text('2')),
  ],
);
```

## 选择原则

### 1. 页面级别

- 使用 `Scaffold` 作为页面的根 Widget
- 提供完整的 Material Design 布局结构

### 2. 内容组织

- 垂直排列用 `Column`
- 水平排列用 `Row`
- 层叠效果用 `Stack`
- 需要换行用 `Wrap`

### 3. 滚动内容

- 列表内容用 `ListView`
- 网格内容用 `GridView`
- 长内容用 `SingleChildScrollView`

### 4. 定位和大小

- 居中用 `Center`
- 自定义对齐用 `Align`
- 固定大小用 `SizedBox`
- 填充剩余空间用 `Expanded`

### 5. 装饰和样式

- 需要边距、边框、背景等用 `Container`
- 需要阴影效果用 `Material`
- 简单的间距用 `SizedBox` 或 `Padding`

## 实际应用示例

```dart
class ProfilePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('个人资料')),
      body: SingleChildScrollView(
        child: Column(
          children: [
            // 头像部分
            Stack(
              children: [
                Container(
                  height: 200,
                  width: double.infinity,
                  color: Colors.blue,
                ),
                Center(
                  child: CircleAvatar(
                    radius: 50,
                    backgroundImage: AssetImage('avatar.jpg'),
                  ),
                ),
              ],
            ),
          
            // 信息部分
            Padding(
              padding: EdgeInsets.all(16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Row(
                    children: [
                      Icon(Icons.person),
                      SizedBox(width: 8),
                      Expanded(
                        child: Text('用户名: KBchulan'),
                      ),
                    ],
                  ),
                  // 更多信息...
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
```

## 重要提示

1. 优先使用语义化的 Widget
2. 考虑内容是否需要滚动
3. 注意性能影响
4. 合理使用组合

---

_更新时间：2025-03-12 12:08:30 UTC_
_作者：KBchulan_
