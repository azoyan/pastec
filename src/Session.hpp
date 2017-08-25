#ifndef SESSION_H
#define SESSION_H

#include <chrono>
#include <string>

namespace pastec {
  using Time = std::chrono::time_point<std::chrono::system_clock>;
  using Key  = std::string;
  using Data = std::string;

  class Session {
  public:
    Session(const Key& key, const Data& data, const std::chrono::seconds& lifeTime) noexcept
    : m_key(key)
    , m_data(data)
    , m_startUpTime(std::chrono::system_clock::now())
    , m_endLifeTime(m_startUpTime + std::chrono::seconds(lifeTime)) { }

    void setData(const Data& data, const std::chrono::seconds& lifeTime) noexcept {
      m_data = data;
      setTime(lifeTime);
    }

    void setTime(const std::chrono::seconds& lifeTime) noexcept {
      m_startUpTime = std::chrono::system_clock::now();
      m_endLifeTime = m_startUpTime + std::chrono::seconds (lifeTime);
    }

  public:
    Key  key()         const noexcept { return m_key;         }
    Data data()        const noexcept { return m_data;        }
    Time startUpTime() const noexcept { return m_startUpTime; }
    Time endTime()     const noexcept { return m_endLifeTime; }

    int remainingTime() const noexcept {
      return std::chrono::duration_cast<std::chrono::seconds>(m_endLifeTime - m_startUpTime).count();
    }

    bool isExpired() const noexcept {
      return std::chrono::system_clock::now() > m_endLifeTime;
    }

  private:
    Key  m_key;
    Data m_data;
    Time m_startUpTime;
    Time m_endLifeTime;
  };
}

#endif // SESSION_H
