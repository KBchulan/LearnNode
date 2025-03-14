// https://docs.flutter.dev/ui

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

class MyAppState extends ChangeNotifier {
  var num = 10;
}

class BorderImage extends StatelessWidget {
  const BorderImage({super.key});

  @override
  Widget build(BuildContext context) {
    return Image(
        image: AssetImage('lib/assets/cursor.jpg'), width: 200, height: 200);
  }
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
        create: (context) => MyAppState(),
        child: MaterialApp(home: MyHomePage()));
  }
}

class MyHomePage extends StatelessWidget {
  const MyHomePage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: ListView(
      children: [
        Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
          Text('Dash1'),
          BorderImage(),
        ]),

        Divider(),

        Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
          Text('Dash2'),
          BorderImage(),
        ]),

        Divider(),

        Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
          Text('Dash3'),
          BorderImage(),
        ]),

        Divider(),

        Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
          Text('Dash4'),
          BorderImage(),
        ]),

        Divider(),

        Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children: [
          Text('Dash5'),
          BorderImage(),
        ]),
      ],
    ));
  }
}

void main() {
  runApp(const MyApp());
}
