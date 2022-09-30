#include "printer_utils_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#include <iostream>

namespace printer_utils {

// static
void PrinterUtilsPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "printer_utils",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<PrinterUtilsPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

PrinterUtilsPlugin::PrinterUtilsPlugin() {}

PrinterUtilsPlugin::~PrinterUtilsPlugin() {}

void PrinterUtilsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  auto method_name = method_call.method_name();
  if (method_name.compare("open") == 0) {
    auto args = std::get<flutter::EncodableMap>(*method_call.arguments());
    std::string printerName = std::get<std::string>(args[flutter::EncodableValue::EncodableValue("name")]);
    BOOL bStatus = FALSE;
    PRINTER_DEFAULTS* pDefault = new PRINTER_DEFAULTS();
    pDefault->DesiredAccess = PRINTER_ALL_ACCESS;

    bStatus = OpenPrinterA(const_cast<char*>(printerName.c_str()), &hPrinter, NULL);
    result->Success(flutter::EncodableValue(bStatus == TRUE ? true: false));
  } else if (method_name.compare("isOpen") == 0) {
      DWORD cbNeeded;
      byte data[128];

      DWORD res = GetPrinterDataW(hPrinter, (wchar_t*)L"NLMOpenDoc", nullptr, data, sizeof(data), &cbNeeded);
      if (res == 0 && isPartOf((char*)data, (char*)"NLMOpenOk")) {
          result->Success(flutter::EncodableValue(true));
      }
      else {
          result->Success(flutter::EncodableValue(false));
      }
  }
  else if (method_name.compare("write") == 0) {
      auto args = std::get<flutter::EncodableMap>(*method_call.arguments());
      auto value = std::get<std::vector<uint8_t>>(args[flutter::EncodableValue::EncodableValue("value")]);
      std::wstring prefix = L"NLMPassThrough";

      std::wstring s_message(value.begin(), value.end());

      //char* s_message = new char[value.size()];
      //std::copy(value.begin(), value.end(), s_message);
      std::wstring property = prefix + s_message;
      const wchar_t* w_property = property.c_str();

      //strcpy_s(property, sizeof(prefix), prefix);
      //strcat_s(property, value.size(), s_message);
      
      //strcat_s(property, strlen(s_message) + 1, s_message);

      std::string data_message(value.begin(), value.end());

      DWORD written;
      LPBYTE message = (LPBYTE)const_cast<char*>(&data_message[0]);

      DWORD res = GetPrinterDataW(hPrinter, (wchar_t*)w_property, nullptr, message, static_cast<DWORD>(value.size()), &written);
      if (res == ERROR_SUCCESS) {
          result->Success(nullptr);
      }
      else {
          result->Error(std::to_string(res), "Unable to write to the printer");
      }

  }
  else if (method_name.compare("read") == 0) {
      wchar_t* property = L"NLMReadData";

      DWORD readed;
      BYTE readData[1280];

      DWORD res = GetPrinterDataW(hPrinter, property, NULL, NULL, 0, &readed);
      if (res == ERROR_MORE_DATA) {
          BYTE pData[sizeof(readed)];
          res = GetPrinterDataW(hPrinter, property, NULL, pData, readed, &readed);
          OutputDebugString(L"Test");
      }

      char* c_data = (char*)readData;
      //char* c_data = new char[readed];
      //memcpy(c_data, readData, readed * sizeof(char));

      flutter::EncodableMap res_data;
      res_data.insert(std::pair<flutter::EncodableValue, int>(flutter::EncodableValue("res"), res));
      if (res == ERROR_SUCCESS) {
          std::vector<uint8_t> data(c_data, c_data + readed / sizeof(char));
          res_data.insert(std::pair<flutter::EncodableValue, std::vector<uint8_t>>(flutter::EncodableValue("data"), data));
      }
      else {
          if (res == 2) {
              readed = 0;
          }
          std::vector<uint8_t> data(c_data, c_data + readed / sizeof(char));
          res_data.insert(std::pair<flutter::EncodableValue, std::vector<uint8_t>>(flutter::EncodableValue("data"), data));
      }
      result->Success(res_data);
  }
  else if (method_name.compare("close") == 0) {
      if (hPrinter != nullptr) {
          byte data[128];
          GetPrinterDataW(hPrinter, (wchar_t*)L"NLMCloseDoc", nullptr, data, sizeof(data), nullptr);

          ClosePrinter(hPrinter);
      }
      hPrinter = nullptr;
      result->Success(nullptr);
  }
  else {
    result->NotImplemented();
  }
}

bool PrinterUtilsPlugin::isPartOf(char* w1, char* w2) {
    int i = 0;
    int j = 0;

    while (w1[i] != '\0') {
        if (w1[i] == w2[j]) {
            while (w1[i] == w2[j] && w2[j] != '\0') {
                j++;
                i++;
            }
            if (w2[j] == '\0') {
                return true;
            }
            j = 0;
        }
        i++;
    }
    return false;
}


}  // namespace printer_utils
