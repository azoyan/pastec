#include "Storage.h"
#include "Dictionary.hpp"
#include <iostream>
#include <cassert>

pastec::Storage::Storage(const char* dictionaryFilePath)
  : m_dictionary(CreateDictionary(dictionaryFilePath))
{
  assert(!m_dictionary.empty());
  m_sessions.reserve(m_dictionary.size());
  m_sessions.rehash(m_dictionary.size());
  std::cout << "Init Storage\n";
}

pastec::Key pastec::Storage::insert(const Data& data, const std::chrono::seconds& duration) noexcept {
  Key result;
  Time time;
  const Time now = Time::clock::now();
  const Time expirationTime = now + duration;
  const std::pair<const pastec::Time, const char*> wrong{now, ""};

  if (std::tie(time, result) = !m_dates.empty() ? *m_dates.begin() : wrong; time < now) {
    auto expiredNode = m_dates.extract(time);
    expiredNode.key() = expirationTime;
    m_dates.insert(std::move(expiredNode));
  }
  else if (const size_t index = m_dates.size(); index < m_dictionary.size()) {
    result = m_dictionary.at(index);
    m_dates.insert(std::make_pair(expirationTime, result));
  }
  m_sessions.insert_or_assign(result, data);
  return result;
}

pastec::Data pastec::Storage::data(const Key& key) const noexcept {
  Data data;
  auto it = m_sessions.find(key);
  if (it != m_sessions.end()) {
    data = it->second;
  }
  return data;
}
