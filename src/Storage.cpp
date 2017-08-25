#include "Storage.h"
#include "Dictionary.hpp"
#include <iostream>
#include <cassert>

pastec::Storage::Storage(const char* dictionaryFilePath)
: m_dictionary(CreateDictionary(dictionaryFilePath))
{
  assert(!m_dictionary.empty());
  m_sessions.reserve(m_dictionary.size());
  std::cout << "Init Storage\n";
}

pastec::Key pastec::Storage::insert(const Data& data, const std::chrono::seconds& lifeTime) noexcept {
  size_t i = 0;
  const size_t count = m_sessions.size();
  while (i < count) {
    Session&   session           = m_sessions.at(i);
    const bool hasSessionExpired = session.isExpired();

    if (hasSessionExpired) {
      session.setData(data, lifeTime);
      return session.key();
    }
    ++i;
  }
  const Key& key = m_dictionary.at(i);
  m_sessions.emplace_back(key, data, lifeTime);
  return key;
}

pastec::Data pastec::Storage::data(const Key& key) const noexcept {
  for (const Session& session : m_sessions) {
    const bool hasKey            = session.key() == key;
    const bool hasSessionExpired = session.isExpired();
    if (hasKey and not hasSessionExpired) {
      return session.data();
    }
  }
  return Data();
}
