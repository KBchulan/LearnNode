// https://docs.flutter.dev/ui

import 'package:flutter/material.dart';

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [BorderImage(), const Text('Dash 1')]),
            SizedBox(width: 20),
            Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [BorderImage(), const Text('Dash 2')]),
            SizedBox(width: 20),
            Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [BorderImage(), const Text('Dash 3')]),
          ],
        ),
      ),
    );
  }
}

class BorderImage extends StatelessWidget {
  const BorderImage({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return Image(
      image: AssetImage('lib/assets/cursor.jpg'),
      width: 200,
      height: 200,
    );
  }
}

void main() {
  runApp(MyApp());
}
