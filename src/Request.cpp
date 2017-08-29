#include <iostream>
#include "Request.h"
#include "Storage.h"

namespace {
  static pastec::Storage s_storage;
}

enum { Kb = 1024 };
enum { MaxPostSize = 5 * Kb };

pastec::Request::Request() : Fastcgipp::Request<char>(MaxPostSize) {
  std::cout << "Request started\n";
}

std::string pastec::Request::getHtml(const std::string& requestUri) {
  std::string result;
  if (!requestUri.empty()) {

      auto data = s_storage.data(requestUri.substr(1));
      if (data.empty()) {
        data = "404 Not found";
      }
      result = "HTTP/1.x 200  \r\n"
               "Content-Type: text/html; charset=utf-8\r\n\r\n"
               "<!DOCTYPE html>"
               "<html>"
               "<body>"
               "<center>"
               "<div style=\"text-align:center;\">"
               + data +
               "</div>"
               "</center>"
               "</form>"
               "</body>"
               "</html>";
    }
    else {
      result = "Content-Type: text/html; charset=utf-8\r\n\r\n"
               "<!DOCTYPE html>"
               "<html>"
               "<body>"
               "<form action=\"/\" id=\"userform\" method=\"post\">"
               "<center>"
               "<textarea rows=\"4\" cols=\"50\" name=\"data\" form=\"userform\"></textarea>"
               "<br>"
               "<input type=\"submit\">"
               "</center>"
               "</form>"
               "</body>"
               "</html>";
    }
    return result;
}

std::string pastec::Request::handlePost(const std::multimap<std::string, std::string>& posts) {
  std::string result;
  for (auto p : posts) {
    std::string url = s_storage.insert(p.second, std::chrono::minutes(1));
    result = "HTTP/1.x 302 \r\n"
             "Location: " + url + "\r\n"
                                  "Content-Type: text/html; charset=utf-8\r\n\r\n";
  }
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


  std::cout << "Сontent type = ["   << environment().contentType << "]\n"
            << "Сontent length = [" << contentLength             << "]\n"
            << "Host = ["           << environment().host        << "]\n"
            << "Request URI = ["    << requestUri                << "]\n"
            << "Request method ["   << requestMethod             << "]\n"
            << "Remote address = [" << remoteAddress             << "]\n"
            << "Remote port = ["    << remotePort                << "]\n"
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
