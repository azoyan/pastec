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
    static std::string get(std::string& requestUri);
    static std::string post(const std::multimap<std::string, std::string>& posts);
    bool response() override;
    std::string createResponse();
  };

  static std::string preload(const char* pagePath);
}

#endif // REQUEST_H
