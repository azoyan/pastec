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

pastec::Key pastec::Storage::insert(const Data& data, const std::chrono::seconds& duration) noexcept {
  Key key;
  const Time now = Time::clock::now();
  const Time expirationTime = now + duration;
  if (auto potential = m_expireDates.cbegin();(!m_expireDates.empty()) && (*potential).first < now) {
    auto expiredNode = m_expireDates.extract((*potential).first);
    expiredNode.key() = expirationTime;
    m_expireDates.insert(std::move(expiredNode));  //обновляем дату-время истечения срока хранения
    key = (*potential).second;
  }
  else {
    key = m_dictionary.at(m_expireDates.size());
    m_expireDates.insert(std::make_pair(expirationTime, key));
  }
  m_sessions.insert_or_assign(key, data);
  return key;
}

pastec::Data pastec::Storage::data(const Key& key) const noexcept {
  Data data;
  auto it = m_sessions.find(key);
  if (it != m_sessions.end()) {
    data = it->second;
  }
  return data;
}
