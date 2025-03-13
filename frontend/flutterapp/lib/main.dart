import 'package:english_words/english_words.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// void main() {
//   runApp(MyApp());
// }

// StatelessWidget
// StatefulWidget
// Layout Widgets (like Container, Row, Column)
// Material Design Widgets
// Cupertino (iOS-style) Widgets
class MyApp extends StatelessWidget {
  const MyApp({super.key});

  /// 创建应用级状态、命名应用、定义视觉主题以及设置"主页" widget，即应用的起点。
  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(

      create: (context) => MyAppState(),

      child: MaterialApp(
        title: 'Namer App',
        theme: ThemeData(
          useMaterial3: true,
          colorScheme: ColorScheme.fromSeed(
              seedColor: const Color.fromARGB(255, 158, 219, 214)),
        ),
        home: MyHomePage(),
      ),
      
    );
  }
}

/// 创建应用状态，这个和pinia的state比较像，定义应用运行所需的数据
class MyAppState extends ChangeNotifier {
  var current = WordPair.random();

  void getNext() {
    current = WordPair.random();
    notifyListeners();
  }

  // 增加喜欢的单词属性
  var favorites = <WordPair>[];

  void changeFavorityWordPairs() {
    if (favorites.contains(current)) {
      favorites.remove(current);
    } else {
      favorites.add(current);
    }
    notifyListeners();
  }
}

/// 主页面组件，包含导航栏和内容区域
class MyHomePage extends StatefulWidget {
  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  var selectedIndex = 0;

  @override
  Widget build(BuildContext context) {
    Widget page;
    switch (selectedIndex) {
      case 0:
        page = GeneratorPage();
        break;
      case 1:
        page = FavoritesPage();
        break;
      default:
        throw UnimplementedError('no widget for $selectedIndex');
    }

    return Scaffold(
      // 内容区域
      body: Container(
        // 使用主题色作为背景
        color: Theme.of(context).colorScheme.primaryContainer,
        child: page,
      ),

      // 底部导航栏
      bottomNavigationBar: BottomNavigationBar(
        // 导航项配置
        items: [
          BottomNavigationBarItem(
            icon: Icon(Icons.home),
            label: 'Home',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.favorite),
            label: 'Favorites',
          ),
        ],

        currentIndex: selectedIndex, // 当前选中的导航项索引

        // 导航项选中回调
        onTap: (value) {
          setState(() {
            // 这样可以通知页面改变了
            selectedIndex = value;
          });
        },
      ),
    );
  }
}

/// 生成器页面，显示随机单词对并提供操作按钮
class GeneratorPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    // 获取应用状态
    var appState = context.watch<MyAppState>();
    var pair = appState.current;

    // 根据是否收藏决定图标样式
    IconData icon;
    if (appState.favorites.contains(pair)) {
      icon = Icons.favorite;
    } else {
      icon = Icons.favorite_border;
    }

    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          // 显示单词对的卡片
          BigCard(pair: pair),
          SizedBox(height: 10),

          Row(
            mainAxisSize: MainAxisSize.min,
            children: [
              // 收藏按钮
              ElevatedButton.icon(
                onPressed: () {
                  appState.changeFavorityWordPairs();
                },
                icon: Icon(icon),
                label: Text('Like'),
              ),

              SizedBox(width: 10),
              // 下一个按钮
              ElevatedButton(
                onPressed: () {
                  appState.getNext();
                },
                child: Text('Next'),
              ),
            ],
          ),
        ],
      ),
    );
  }
}

class FavoritesPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    var appState = context.watch<MyAppState>();
    var favorites = appState.favorites;

    if (favorites.isEmpty) {
      return Center(
        child: Text('you have no favorites'),
      );
    }

    return ListView(
      children: [
        Padding(
          padding: const EdgeInsets.all(20),
          child: Text('You have '
              '${appState.favorites.length} favorites:'),
        ),
        ...favorites.map((pair) => ListTile(
            leading: Icon(Icons.favorite), title: Text(pair.asLowerCase)))
      ],
    );
  }
}

class BigCard extends StatelessWidget {
  const BigCard({
    super.key,
    required this.pair,
  });

  //
  final WordPair pair;

  @override
  Widget build(BuildContext context) {
    // 卡片颜色主题
    final theme = Theme.of(context);
    // 文字大小及颜色
    final style = theme.textTheme.displayMedium!
        .copyWith(color: Color.fromARGB(255, 206, 193, 126));

    return Card(
      color: theme.colorScheme.primary,
      shadowColor: Color.fromARGB(255, 131, 184, 131),

      /// 主题方案的基本
      child: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Text(
          pair.asLowerCase,
          style: style,
          semanticsLabel: "${pair.first} ${pair.second}",

          /// 将语义标签这样设置
        ),
      ),
    );
  }
}
