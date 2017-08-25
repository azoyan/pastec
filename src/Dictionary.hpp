#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>

namespace pastec {
  inline std::vector<std::string> CreateDictionary(const char* dictioniaryFilePath) {
    std::vector<std::string> result;
    std::ifstream file(dictioniaryFilePath);
    assert(file.is_open());
    std::string line;
    while (std::getline(file, line)) {
      result.push_back(line);
    }
    file.close();
    return result;
  }
}

#endif // DICTIONARY_H
