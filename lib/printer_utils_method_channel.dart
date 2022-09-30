import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'printer_utils_platform_interface.dart';

/// An implementation of [PrinterUtilsPlatform] that uses method channels.
class MethodChannelPrinterUtils extends PrinterUtilsPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('printer_utils');

  @override
  Future<bool> open(String printerName) async {
    final isOpen = await methodChannel.invokeMethod<bool>('open', {"name": printerName});
    return isOpen!;
  }

  @override
  Future<void> close() async {
    await methodChannel.invokeMethod('close');
  }

  @override
  Future<bool> isOpen() async {
    final isOpen = await methodChannel.invokeMethod<bool>('isOpen');
    return isOpen!;
  }

  @override
  Future<void> write(List<int> data) async {
    await methodChannel.invokeMethod('write', {"value": data});
  }

  @override
  Future<Map<String, dynamic>?> read() async {
    final data = await methodChannel.invokeMethod<dynamic>('read');
    return Map.from(data);
  }
}
