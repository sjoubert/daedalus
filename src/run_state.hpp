#ifndef DAEDALUS_RUN_STATE_HPP
#define DAEDALUS_RUN_STATE_HPP

#include <random>

namespace daedalus
{

class RunState
{
public:
  std::size_t newWidth();
  std::size_t newHeight();
  void increaseSize();

  float getTimeFactor() const;
  void increaseTimeFactor();

  bool hasBonus() const;
  void setBonus(bool p_bonus);

private:
  std::mt19937 m_rng{std::random_device{}()};
  std::uniform_int_distribution<std::size_t> m_sizeDist{10, 15};

  float m_timeFactor{0.5};

  bool m_bonus{false};
};

}

#endif
