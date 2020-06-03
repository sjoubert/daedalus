#ifndef DAEDALUS_RUN_STATE_HPP
#define DAEDALUS_RUN_STATE_HPP

#include "item.hpp"

#include <random>
#include <vector>
#include <string>
#include <functional>

namespace daedalus
{

class RunState
{
public:
  RunState();

  std::size_t newWidth();
  std::size_t newHeight();
  void increaseSize();

  float getTimeFactor() const;
  void increaseTimeFactor();

  std::vector<Item> getBonus();
  void addBonus();
  std::vector<Item> getMalus();

  int currentLevel() const;
  void nextLevel(std::vector<Item> const& p_bonus, std::vector<Item> const& p_malus);

private:
  void initItems();

  std::vector<Item> m_bonus;
  std::vector<Item> m_malus;

  std::mt19937 m_rng{std::random_device{}()};
  std::uniform_int_distribution<std::size_t> m_sizeDist{10, 15};

  float m_timeFactor{0.5};

  std::mt19937 m_itemRng{std::random_device{}()};
  std::size_t m_bonusCount{0};

  unsigned int m_level{1};
};

}

#endif
