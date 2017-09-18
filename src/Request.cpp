 #include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "Request.h"
#include "Storage.h"

namespace {
  static pastec::Storage s_storage;

  static std::ifstream start("start.html");
  static std::string startPage = std::string("Content-Type: text/html; charset=utf-8\r\n\r\n")
       + std::string(std::istreambuf_iterator<char>(start), std::istreambuf_iterator<char>());

  static std::ifstream res("result.html");
  static const std::string resultPage = std::string("HTTP/1.x 200  \r\nContent-Type: text/html; charset=utf-8\r\n\r\n") +
             std::string(std::istreambuf_iterator<char>(res), std::istreambuf_iterator<char>());

  static std::regex urlRegex(".*\\..*");
}

enum { Kb = 1024, Mb = Kb * Kb };
enum { MaxPostSize = 10 * Mb };

pastec::Request::Request() : Fastcgipp::Request<char>(MaxPostSize) {
  std::cout << "Request started\n";
}

std::string pastec::Request::getHtml(const std::string& requestUri) {
  std::string result = startPage;
  if (!requestUri.substr(1).empty()) {
    std::string data = s_storage.data(requestUri.substr(1));
    if (data.empty()) {
      data = "This page has been never created or time expired!";
    }
    data = std::regex_replace(data, urlRegex, R"(<a href="$&" target="_blank">$&</a>)");
    result = resultPage;
    const std::string t = "{{data}}";
    const uint f = resultPage.find(t);
    result.replace(f, t.length(), data);
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

  const std::string url = s_storage.insert(userData, std::chrono::seconds(lifetime));
  result = "HTTP/1.x 302 \r\n" "Location: " + url + "\r\n"
                                  "Content-Type: text/html; charset=utf-8\r\n\r\n";
  return result;
}

std::string pastec::Request::errorHtml() {
  std::string result;
  result = "HTTP/1.x 200  \r\n"
           "Content-Type: text/html; charset=utf-8\r\n\r\n"
           "<!DOCTYPE html>"
           "<html>"
           "<body>"
           "<center>"
           "<div style=\"text-align:center;\">"
           "ERROR"
           "</div>"
           "</center>"
           "</form>"
           "</body>"
           "</html>";
  return result;
}

std::string pastec::Request::createHtml() {
  using namespace Fastcgipp::Http;
  const RequestMethod requestMethod = environment().requestMethod;
  const std::string requestUri      = environment().requestUri;
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
