#pragma once

#include "Application.hpp"

int main() {
  auto app = Hydrogen::CreateApplication();

  app->Run();
  HY_LOG_DEBUG("Finished running client application!");
}
