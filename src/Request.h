#ifndef REQUEST_H
#define REQUEST_H

#include <experimental/filesystem>
#include <fastcgi++/request.hpp>
#include <string>

namespace  pastec {
  enum class Language : uint8_t { English, Russian };

  class Request : public Fastcgipp::Request<char> {
  public:
    Request();

    bool response() override;
    std::string createHtml();
  };
}

#endif // REQUEST_H
