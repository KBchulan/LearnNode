# Flutter Widget 继承体系完整指南

## 基础继承链

```dart
Object
  └── DiagnosticableTree
      └── Widget
          ├── StatelessWidget
          ├── StatefulWidget
          └── RenderObjectWidget
```

## 核心Widget类型

```dart
Widget
├── StatelessWidget    // 无状态Widget
├── StatefulWidget     // 有状态Widget
└── RenderObjectWidget // 渲染对象Widget
    └── LeafRenderObjectWidget
    └── SingleChildRenderObjectWidget
    └── MultiChildRenderObjectWidget
```

## 详细分类

### 1. 布局类Widgets (Layout)

```dart
- Container     // 容器
- Row          // 水平布局
- Column       // 垂直布局
- Stack        // 层叠布局
- Expanded     // 填充剩余空间
- Flexible     // 弹性布局
- Wrap         // 自动换行
- Flow         // 流式布局
- LayoutBuilder // 布局构建器
- CustomMultiChildLayout
- CustomSingleChildLayout
```

### 2. 内容容器 (Content)

```dart
- Text         // 文本
- Image        // 图片
- Icon         // 图标
- Button系列
  ├── TextButton
  ├── ElevatedButton
  ├── OutlinedButton
  └── IconButton
```

### 3. 滚动组件 (Scrolling)

```dart
- ListView
- GridView
- SingleChildScrollView
- CustomScrollView
- PageView
- ScrollConfiguration
```

### 4. 功能性组件 (Functional)

```dart
- GestureDetector   // 手势检测
- InkWell          // 水波纹效果
- Draggable        // 可拖拽
- Hero             // 页面过渡动画
- Navigator        // 导航
```

### 5. 对话框和弹出窗口 (Dialogs & Popups)

```dart
- Dialog
- AlertDialog
- SimpleDialog
- BottomSheet
- PopupMenuButton
- SnackBar
```

### 6. 输入组件 (Input)

```dart
- TextField
- Form
- FormField
- TextFormField
- Checkbox
- Radio
- Switch
- Slider
```

### 7. 信息展示 (Information Display)

```dart
- Card
- Chip
- Tooltip
- ProgressIndicator
  ├── CircularProgressIndicator
  └── LinearProgressIndicator
```

### 8. 动画组件 (Animation)

```dart
- AnimatedContainer
- AnimatedOpacity
- AnimatedPositioned
- TweenAnimationBuilder
- FadeTransition
- SlideTransition
```

### 9. Material Design专用

```dart
- MaterialApp
- Scaffold
- AppBar
- BottomNavigationBar
- FloatingActionButton
- TabBar
- Drawer
```

### 10. Cupertino (iOS风格) 组件

```dart
- CupertinoApp
- CupertinoPageScaffold
- CupertinoNavigationBar
- CupertinoButton
- CupertinoAlertDialog
```

### 11. 状态管理Widget

```dart
- InheritedWidget
- StreamBuilder
- FutureBuilder
- ValueListenableBuilder
```

## 使用建议

1. 应用入口：

```dart
MaterialApp或CupertinoApp
```

2. 基础页面结构：

```dart
Scaffold
  ├── AppBar
  ├── Body
  ├── FloatingActionButton
  ├── Drawer
  └── BottomNavigationBar
```

3. 常用布局组合：

```dart
Container
  └── Column/Row
      ├── Expanded/Flexible
      └── 其他Widget
```

4. 列表展示：

```dart
ListView.builder或GridView.builder
```

5. 用户交互：

```dart
GestureDetector或InkWell
  └── 其他Widget
```
