#include "run_state.hpp"

namespace daedalus
{

#define DAEDALUS_ITEM(p_container, p_uses, p_name, p_text) \
  p_container.emplace_back(p_uses, p_name, p_text).getEffect() = [](RunState& state)
#define DAEDALUS_BONUS(...) DAEDALUS_ITEM(m_bonus, __VA_ARGS__)
#define DAEDALUS_MALUS(...) DAEDALUS_ITEM(m_malus, __VA_ARGS__)

void RunState::initItems()
{
  DAEDALUS_BONUS(-1, "Hourglass", "Take your sweet time")
  {
    state.m_timeFactor += 0.1;
  };
  DAEDALUS_MALUS(-1, "Metronome", "Harder? Better? Faster!")
  {
    state.m_timeFactor -= 0.1;
  };

  DAEDALUS_BONUS(-1, "Zip", "Hey, I shrunk the maze")
  {
    state.m_sizeDist =
      std::uniform_int_distribution<std::size_t>{state.m_sizeDist.min() - 1, state.m_sizeDist.max() - 1};
  };
  DAEDALUS_MALUS(-1, "Unzip", "It's bigger on the inside")
  {
    state.m_sizeDist =
      std::uniform_int_distribution<std::size_t>{state.m_sizeDist.min() + 1, state.m_sizeDist.max() + 1};
  };

  DAEDALUS_BONUS(1, "Flashlight", "See far away")
  {
    state.addItem(Item::Id::Flashlight);
  };

  DAEDALUS_BONUS(1, "Treasure map", "Oh, the shiny things")
  {
    state.addItem(Item::Id::TreasureMap);
  };
  DAEDALUS_BONUS(1, "Compass", "You are my North, mmm..., my exit")
  {
    state.addItem(Item::Id::Compass);
  };
  DAEDALUS_BONUS(1, "Dowser's wand", "I feel water, mmm..., I think")
  {
    state.addItem(Item::Id::DowserWand);
  };
}

}
