// https://docs.flutter.dev/ui

import 'package:flutter/material.dart';

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
      body: Row(
        children: [
          Expanded(child: BorderImage()),
          Expanded(child: BorderImage()),
          Expanded(child: BorderImage())
        ],
      ),
    ));
  }
}

class BorderImage extends StatelessWidget {
  const BorderImage({super.key});

  @override
  Widget build(BuildContext context) {
    return Image(
      image: AssetImage('lib/assets/cursor.jpg'),
      width: 450,
      height: 450,
    );
  }
}

void main() {
  runApp(MyApp());
}
