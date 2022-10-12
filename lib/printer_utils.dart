import 'printer_utils_platform_interface.dart';

class PrinterUtils {
  Future<bool> open(String printerName) {
    return PrinterUtilsPlatform.instance.open(printerName);
  }

  Future<void> close() {
    return PrinterUtilsPlatform.instance.close();
  }

  Future<bool> isOpen() {
    return PrinterUtilsPlatform.instance.isOpen();
  }

  Future<void> write(String data) {
    return PrinterUtilsPlatform.instance.write(data);
  }

  Future<Map<String, dynamic>?> read() async {
    return PrinterUtilsPlatform.instance.read();
  }
}
