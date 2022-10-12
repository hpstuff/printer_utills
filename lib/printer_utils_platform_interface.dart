import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'printer_utils_method_channel.dart';

abstract class PrinterUtilsPlatform extends PlatformInterface {
  /// Constructs a PrinterUtilsPlatform.
  PrinterUtilsPlatform() : super(token: _token);

  static final Object _token = Object();

  static PrinterUtilsPlatform _instance = MethodChannelPrinterUtils();

  /// The default instance of [PrinterUtilsPlatform] to use.
  ///
  /// Defaults to [MethodChannelPrinterUtils].
  static PrinterUtilsPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [PrinterUtilsPlatform] when
  /// they register themselves.
  static set instance(PrinterUtilsPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<bool> open(String printerName) {
    throw UnimplementedError('open() has not been implemented.');
  }

  Future<void> close() {
    throw UnimplementedError('close() has not been implemented.');
  }

  Future<bool> isOpen() {
    throw UnimplementedError('isOpen() has not been implemented.');
  }

  Future<void> write(String data) {
    throw UnimplementedError('write() has not been implemented.');
  }

  Future<Map<String, dynamic>?> read() async {
    throw UnimplementedError('read() has not been implemented.');
  }
}
