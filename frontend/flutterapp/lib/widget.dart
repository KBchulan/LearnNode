// https://docs.flutter.dev/ui

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

class MyAppState extends ChangeNotifier {
  var num = 0;
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
        create: (context) => MyAppState(),
        child: MaterialApp(
          home: MyHomePage(),
          theme: ThemeData.from(
              colorScheme: ColorScheme.fromSeed(
                  seedColor: Color.fromARGB(255, 129, 196, 170))),
        ));
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  var num = 0;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: Center(
      child: ElevatedButton(
        onPressed: () {
          setState(() {
            num++;
            print(num);
          });
        },
        child: Text('$num'),
      ),
    ));
  }
}

// void main() {
//   runApp(const MyApp());
// }
