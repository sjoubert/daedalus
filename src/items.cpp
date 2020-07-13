#include "run_state.hpp"

namespace daedalus
{

void RunState::initItems()
{
  // Size
  m_bonus.emplace_back(Item::Id::Hourglass, -1, "Hourglass", "Take your sweet time");
  m_malus.emplace_back(Item::Id::Metronome, -1, "Metronome", "Harder? Better? Faster!");

  // Time
  m_bonus.emplace_back(Item::Id::Hourglass, -1, "Zip", "Hey, I shrunk the maze");
  m_malus.emplace_back(Item::Id::Metronome, -1, "Unzip", "It's bigger on the inside");

  // Visibility
  m_bonus.emplace_back(Item::Id::Flashlight, 1, "Flashlight", "See far away");

  // Locators
  m_bonus.emplace_back(Item::Id::TreasureMap, 1, "Treasure map", "Oh, the shiny things");
  m_bonus.emplace_back(Item::Id::Compass, 1, "Compass", "You are my North, mmm..., my exit");
  m_bonus.emplace_back(Item::Id::DowserWand, 1, "Dowser's wand", "I feel water, mmm..., I think");

  // Movement
  m_malus.emplace_back(Item::Id::Mirror, 1, "Mirror", "The other left");
  m_malus.emplace_back(Item::Id::UpsideDown, 1, "Upside down", "Ups and downs");

  // Items
  m_bonus.emplace_back(Item::Id::Panacea, 1, "Panacea", "So many things");
}

}
