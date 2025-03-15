// https://docs.flutter.dev/ui

import 'dart:ffi';

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

      child: MaterialApp(
        home: MyCounter()
      )
    );
  }
}

class MyCounter extends StatefulWidget{
  const MyCounter({super.key});

  @override
  State<MyCounter> createState() => _MyCounterState();
}

class _MyCounterState extends State<MyCounter> {
  int num = 0;

  @override
  Widget build(BuildContext context) {
    

    return Scaffold(
      body: Container(
        alignment: Alignment.center,
        // 简单的美化设置
        decoration: BoxDecoration(
          color: Colors.lightBlue
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            ElevatedButton(
              onPressed: () {
                setState(() {
                  num++;
                  print(num);
                });
              },  
              child: Text('当前计数: $num'),
            )
          ],
        )
      ),
    );
  }
}

void main() {
  runApp(const MyApp());
}
