#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <chrono>
#include <unordered_map>
#include <map>

namespace pastec {
  using Time = std::chrono::time_point<std::chrono::system_clock>;
  using Key  = std::string;
  using Data = std::string;

  class Storage {
  public:
    Storage(const char* dictionaryFilePath = "../etc/google-10000-english-usa-no-swears.txt");

    Key  insert(const Data& data, const std::chrono::seconds& duration) noexcept;
    Data data(const Key& key) const noexcept;

  private:
    std::vector<std::string> m_dictionary;
    std::unordered_map<Key, Data> m_sessions;
    std::map<Time, Key> m_expireDates;
  };
}

#endif // STORAGE_H
