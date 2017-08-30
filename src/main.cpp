#include <fastcgi++/manager.hpp>
#include "Request.h"
#include <cassert>

int main(int argc, char *argv[]) {
  std::string firstArg;
  std::vector<std::string> args;

  if (argc > 1) {
    firstArg = argv[1];
    args.assign(argv + 1, argv + argc);
  }



  Fastcgipp::Manager<pastec::Request> manager;
  manager.setupSignals();
  bool ok = manager.listen(args.at(0).c_str(), args.at(1).c_str());
  assert(ok);
  manager.start();
  manager.join();
  return 0;
}
