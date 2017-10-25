#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "Request.h"
#include "Storage.h"

namespace {
  static pastec::Storage s_storage;
  static const std::string startPage = pastec::preload("start.html");

  static std::string resultPage   = pastec::preload("result.html");
  static std::string notFoundPage = pastec::preload("notfound.html");

  static const std::string headerOk       = pastec::preload("200.txt");
  static const std::string headerNotFound = pastec::preload("404.txt");
  static const std::string headerCreated  = pastec::preload("201.txt");

  static std::regex urlRegex(R"(@(https?|ftp)://(-\.)?([^\s/?\.#-]+\.?)+(/[^\s]*)?$@iS)");
}

enum { Kb = 1024, Mb = Kb * Kb };
enum { MaxPostSize = 10 * Mb };

pastec::Request::Request() : Fastcgipp::Request<char>(MaxPostSize) {
  std::cout << "Request started\n";
}

std::string pastec::Request::get(std::string& requestUri) {
  if (!requestUri.empty()) {
    requestUri = requestUri.substr(1);
  }

  if (requestUri.empty()) {
    return headerOk + startPage;
  }

  std::string result;
  if (const std::string userdata = s_storage.data(requestUri); !userdata.empty()) {
    const std::string t = "{{userdata}}";
    const uint pos = resultPage.find(t);
    resultPage.replace(pos, t.length(), userdata);
    result = headerOk + resultPage;
  }
  else {
    result = headerNotFound + notFoundPage;
  }
  return result;
}

std::string pastec::Request::post(const std::multimap<std::string, std::string>& posts) {
  std::string result;
  uint lifetime = 15;
  auto it = posts.find("min");
  if (it != posts.end()) {
    lifetime = std::stoi(it->second);
  }
  const std::string& userData = posts.find("data")->second;
  const std::string url = s_storage.insert(userData, std::chrono::minutes(lifetime));
  if (!url.empty()) {
    result = headerCreated;
    const std::string t = "{{url}}";
    const uint pos = headerCreated.find(t);
    result.replace(pos, t.length(), url);
  }
  return result;
}

std::string pastec::Request::createResponse() {
  using namespace Fastcgipp::Http;
  const RequestMethod requestMethod = environment().requestMethod;
  std::string requestUri            = environment().requestUri;
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
            << "Files count = ["    << environment().files.size() << "]\n"
            << std::endl;

  std::string result;
  switch(requestMethod) {
    case RequestMethod::GET:  result = get(requestUri); break;
    case RequestMethod::POST: result = post(environment().posts); break;
    default: break;
  }
  return result;
}

bool pastec::Request::response() {
  const std::string& response = createResponse();
  out << response;
  return true;
}

std::string pastec::preload(const char* pagePath) {
  std::ifstream stream(pagePath);
  const std::string result = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
  return result;
}
