#ifndef FLUTTER_PLUGIN_PRINTER_UTILS_PLUGIN_H_
#define FLUTTER_PLUGIN_PRINTER_UTILS_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace printer_utils {


class PrinterUtilsPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  PrinterUtilsPlugin();

  virtual ~PrinterUtilsPlugin();

  // Disallow copy and assign.
  PrinterUtilsPlugin(const PrinterUtilsPlugin&) = delete;
  PrinterUtilsPlugin& operator=(const PrinterUtilsPlugin&) = delete;

  HANDLE hPrinter;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  bool isPartOf(char* w1, char* w2);
};

}  // namespace printer_utils

#endif  // FLUTTER_PLUGIN_PRINTER_UTILS_PLUGIN_H_
