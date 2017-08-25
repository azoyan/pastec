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

std::string pastec::Request::createHtml() {
  std::cout << "Request:\n" << environment().contentType << "\n"
            << environment().host << "\n"
            << environment().requestUri  << "\n"
            << environment().requestMethod << "\n"
            << std::endl;

  std::string result;

  Fastcgipp::Http::RequestMethod requestMethod = environment().requestMethod;
  if (requestMethod ==  Fastcgipp::Http::RequestMethod::GET) {
    std::string requestUri = std::string(environment().requestUri).substr(1);
    if (!requestUri.empty()) {
      auto data = s_storage.data(requestUri);
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
  }
  else if (requestMethod == Fastcgipp::Http::RequestMethod::POST) {
    for (auto p : environment().posts) {
      std::string url = s_storage.insert(p.second, std::chrono::minutes(1));
      result = "HTTP/1.x 302 \r\n"
               "Location: " + url + "\r\n"
               "Content-Type: text/html; charset=utf-8\r\n\r\n";
    }
  }
  return result;
}

bool pastec::Request::response() {
  const std::string& response = createHtml();
  out << response;
  return true;
}
