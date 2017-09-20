#include <fastcgi++/manager.hpp>
#include "Request.h"
#include <cassert>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  std::ifstream ifs("config.json");
  rapidjson::IStreamWrapper isw(ifs);
  rapidjson::Document configJson;
  configJson.ParseStream(isw);

  const char* ip   = configJson["connection"]["ip"].GetString();
  const char* port = configJson["connection"]["port"].GetString();

  Fastcgipp::Manager<pastec::Request> manager;
  manager.setupSignals();
  bool ok = manager.listen(ip, port);
  if (ok) {
    manager.start();
    manager.join();
  }
  return 0;
}
