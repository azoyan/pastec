#include <fastcgi++/manager.hpp>
#include "Request.h"
#include <cassert>

int main() {
    Fastcgipp::Manager<pastec::Request> manager;
    manager.setupSignals();
    bool ok = manager.listen("localhost", "8000");
    assert(ok);
    manager.start();
    manager.join();
    return 0;
}
