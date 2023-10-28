#pragma once

// #define NOMINMAX
// #include <windows.h>
#include "../../Core/Application.hpp"
#include "../../Core/Logger.hpp"

// int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
int main() {
  auto app = Hydrogen::CreateApplication();

  app->Run();
  HY_LOG_DEBUG("Finished running client application!");
}
