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

  float getTimeFactor() const;

  std::vector<Item> getBonus();
  void addBonus();
  std::vector<Item> getMalus();

  int currentLevel() const;
  void nextLevel(std::vector<Item> const& p_bonus, std::vector<Item> const& p_malus);

  void addItem(Item::Id p_id);
  bool hasItem(Item::Id p_id) const;
  std::size_t countItem(Item::Id p_id) const;

private:
  void initItems();
  std::vector<Item> getItems(std::vector<Item>& p_itemPool, std::size_t p_count);

  std::vector<Item> m_bonus;
  std::vector<Item> m_malus;

  std::mt19937 m_sizeRng{std::random_device{}()};

  std::mt19937 m_itemRng{std::random_device{}()};
  std::size_t m_bonusCount{0};

  unsigned int m_level{1};

  std::vector<Item::Id> m_items;
};

}

#endif
