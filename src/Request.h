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
    static std::string getHtml(std::string& requestUri);
    static std::string handlePost(const std::multimap<std::string, std::string>& posts);
    static std::string errorHtml();
    bool response() override;
    std::string createHtml();
  };

  static std::string preloadPage(const char* pagePath);
}

#endif // REQUEST_H
