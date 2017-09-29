#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "Request.h"
#include "Storage.h"

namespace {
  static pastec::Storage s_storage;
  static const std::string startPage    = pastec::preloadPage("start.html");
  static const std::string resultPage   = pastec::preloadPage("result.html");
  static const std::string notFoundPage = pastec::preloadPage("notfound.html");

  static std::regex urlRegex(R"(@(https?|ftp)://(-\.)?([^\s/?\.#-]+\.?)+(/[^\s]*)?$@iS)");
}

enum { Kb = 1024, Mb = Kb * Kb };
enum { MaxPostSize = 10 * Mb };

pastec::Request::Request() : Fastcgipp::Request<char>(MaxPostSize) {
  std::cout << "Request started\n";
}

std::string pastec::Request::getHtml(std::string& requestUri) {
  std::string result;
  if (!requestUri.empty()) {
    requestUri = requestUri.substr(1);
  }
  if (!requestUri.empty()) {
    const std::string data = s_storage.data(requestUri);
    if (!data.empty()) {
      result = resultPage;
      const std::string t = "{{data}}";
      const uint pos = resultPage.find(t);
      result.replace(pos, t.length(), data);
    }
    else {
      result = notFoundPage;
    }
  }
  else {
    result = startPage;
  }
  return result;
}

std::string pastec::Request::handlePost(const std::multimap<std::string, std::string>& posts) {
  std::string result;
  uint lifetime = 15;
  auto it = posts.find("min");
  if (it != posts.end()) {
    lifetime = std::stoi(it->second);
  }
  const std::string& userData = posts.find("data")->second;

  //  for (auto [key, value] : posts) std::cout << key << " " << value << "\n";

  const std::string url = s_storage.insert(userData, std::chrono::hours(lifetime));
  if (!url.empty()) {
    result = "HTTP/1.x 302 \r\n" "Location: " + url + "\r\n"
                                                      "Content-Type: text/html; charset=utf-8\r\n\r\n";
  }
  else {
    // result =error page'
  }
  return result;
}

std::string pastec::Request::errorHtml() {
  return notFoundPage;
}

std::string pastec::Request::createHtml() {
  using namespace Fastcgipp::Http;
  const RequestMethod requestMethod = environment().requestMethod;
  std::string requestUri      = environment().requestUri;
  const unsigned int contentLength  = environment().contentLength;
  const Address remoteAddress       = environment().remoteAddress;
  const uint16_t remotePort         = environment().remotePort;
  const Address serverAddress       = environment().serverAddress;
  const uint16_t serverPort         = environment().serverPort;

  std::cout << "Сontent type = ["   << environment().contentType << "]\n"
            << "Сontent length = [" << contentLength             << "]\n"
            << "Host = ["           << environment().host        << "]\n"
            << "Request URI = ["    << requestUri                << "]\n"
            << "Request method = [" << requestMethod             << "]\n"
            << "Remote address = [" << remoteAddress             << "]\n"
            << "Remote port = ["    << remotePort                << "]\n"
            << "Server address = [" << serverAddress             << "]\n"
            << "Server port = ["    << serverPort                << "]\n"
            << "Files count = ["    << environment().files.size()<< "]\n"
            << std::endl;

  std::string result;
  switch(requestMethod) {
    case RequestMethod::ERROR: result = errorHtml(); break;
    case RequestMethod::GET:   result = getHtml(requestUri); break;
    case RequestMethod::POST:  result = handlePost(environment().posts); break;
    default: break;
  }
  return result;
}

bool pastec::Request::response() {
  const std::string& response = createHtml();
  out << response;
  return true;
}

std::string pastec::preloadPage(const char* pagePath) {
  std::ifstream stream(pagePath);
  const std::string result = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
  return result;
}
