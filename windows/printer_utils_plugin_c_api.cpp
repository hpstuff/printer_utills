#include "include/printer_utils/printer_utils_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "printer_utils_plugin.h"

void PrinterUtilsPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  printer_utils::PrinterUtilsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
