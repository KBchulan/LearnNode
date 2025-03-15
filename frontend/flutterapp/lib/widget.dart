// https://docs.flutter.dev/ui

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

class MyAppState extends ChangeNotifier {
  var strs = <String>[
    '我是字符串1',
    '我是字符串2',
    '我是字符串3',
    '我是字符串4',
    '我是字符串5',
    '我是字符串6',
    '我是字符串7',
  ];
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
        create: (context) => MyAppState(), // create a global context

        child: MaterialApp(home: MyHomePage()));
  }
}

class MyHomePage extends StatelessWidget {
  const MyHomePage({super.key});

  @override
  Widget build(BuildContext context) {
    var strs = context.watch<MyAppState>().strs;

    return Scaffold(
        body: ListView.builder(
      itemCount: strs.length,
      itemBuilder: (context, index) {
        return Text(strs[index]);
      },
    )
    );
  }
}

void main() {
  runApp(const MyApp());
}
