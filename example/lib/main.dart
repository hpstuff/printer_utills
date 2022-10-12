import 'dart:convert';
import 'dart:ffi';
import 'dart:io';

import 'package:ffi/ffi.dart';
import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:path_provider/path_provider.dart';
import 'package:printer_utils/printer_utils.dart';
import 'package:printer_utils_example/ep_patcher.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final _printerUtilsPlugin = PrinterUtils();

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    //""

    try {
      final name = "ZDesigner ZD621R-300dpi ZPL";
      final ep = NativeLibrary(DynamicLibrary.open('patcher.dll'));
      final handle = ep.Init(name.toNativeChar());
      final isOpen = await _printerUtilsPlugin.open(name);
      //final isOpen = await _printerUtilsPlugin.open("ZDesigner ZD621R-300dpi ZPL");
      print("isOpen: $isOpen");
      if (isOpen) {
        final isConnectionOpen = await _printerUtilsPlugin.isOpen();
        print("isConnectionOpen: $isConnectionOpen");
        if (isOpen && isConnectionOpen) {
          Directory tempDir = await getTemporaryDirectory();
          String tempPath = tempDir.path;
          final file = File('$tempPath\\_zsdk_._zsdk_');
          await file.writeAsBytes(utf8.encode('{}{"device.languages":"null"}'));
          print("file path: ${file.absolute.path}");
          await _printerUtilsPlugin.write(file.absolute.path);
          print("message sended to the printer");
          // final data = await _printerUtilsPlugin.read();
          // print(data);
          int res = 0;
          do {
            final data = await _printerUtilsPlugin.read();
            print(data);
            print(utf8.decode(data?["data"], allowMalformed: true));
            res = data?["res"];
          } while (res == 2);
          await _printerUtilsPlugin.close();
          ep.Cleanup(handle);
        } else {
          print("is connection not open");
        }
      } else {
        print("failed to open printer");
      }
    } catch (e) {
      print(e);
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(),
      ),
    );
  }
}

extension StringExtensions on String {
  Pointer<Char> toNativeChar() {
    return toNativeUtf8().cast<Char>();
  }
}
