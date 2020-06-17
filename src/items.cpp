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
  DAEDALUS_MALUS(-1, "Unzip", "Maze size increase")
  {
    state.m_sizeDist =
      std::uniform_int_distribution<std::size_t>{state.m_sizeDist.min() + 1, state.m_sizeDist.max() + 1};
  };

  DAEDALUS_BONUS(1, "Flashlight", "See far away")
  {
    state.setFlashlight(true);
  };
}

}
