#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include "Session.hpp"

namespace pastec {
  class Storage {
  public:
    Storage(const char* dictionaryFilePath = "../etc/google-10000-english-usa-no-swears.txt");

    Key  insert(const Data& data, const std::chrono::seconds& lifeTime) noexcept;
    Data data(const Key& key) const noexcept;

  private:
    std::vector<std::string> m_dictionary;
    std::vector<Session> m_sessions;
  };
}

#endif // STORAGE_H
