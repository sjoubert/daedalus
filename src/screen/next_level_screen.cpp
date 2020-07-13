#include "next_level_screen.hpp"

#include "level_screen.hpp"
#include "resources.hpp"
#include "gui.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <algorithm>

namespace daedalus
{

NextLevelScreen::NextLevelScreen(sf::RenderWindow& p_window, sf::Texture p_background, RunState p_state)
  : Screen(p_window)
  , m_background(p_background)
  , m_backgroundSprite(m_background)
  , m_state(std::move(p_state))
{
}

std::unique_ptr<Screen> NextLevelScreen::run()
{
  float nextLevelButtonSpacing{};

  auto bonus = m_state.getBonus();
  auto malus = m_state.getMalus();
  malus.front().getSelected() = true;

  sf::SoundBuffer tickTockBuffer;
  tickTockBuffer.loadFromFile(getResource("audio/ta-da.wav"));
  sf::Sound tickTockSound(tickTockBuffer);
  tickTockSound.play();

  sf::Clock deltaClock;
  while (getWindow().isOpen())
  {
    sf::Event event;
    while (getWindow().pollEvent(event))
    {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed)
      {
        getWindow().close();
      }
    }

    ImGui::SFML::Update(getWindow(), deltaClock.restart());

    ImGui::SetNextWindowSize({getWindow().getSize().x / 2.f, -1});
    ImGui::Begin("next_level_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    ImGui::Columns(2, nullptr, false);

    if (bonus.empty())
    {
      ImGui::Text("No bonus found!");
    }
    else
    {
      ImGui::Text("Bonus (%lu available):", m_state.getBonusCount());
      for (auto& item: bonus)
      {
        ImGui::Bullet();
        ImGui::Selectable(item.getName().c_str(), &item.getSelected());
        if (ImGui::IsItemHovered())
        {
          ImGui::SetTooltip("%s", item.getText().c_str());
        }
      }
    }

    ImGui::NextColumn();

    ImGui::Text("Malus (1 required):");
    for (auto& item: malus)
    {
      ImGui::Bullet();
      ImGui::Selectable(item.getName().c_str(), &item.getSelected());
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("%s", item.getText().c_str());
      }
    }

    ImGui::Columns(1);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Dummy({nextLevelButtonSpacing, -1});
    ImGui::SameLine();
    auto selectedMalus = std::ranges::count_if(malus, [](Item const& p_item)
    {
      return p_item.isSelected();
    });
    if (selectedMalus != 1)
    {
      ImGui::Text("Select your bonus and malus");
    }
    else
    {
      if (Gui::SoundButton("Next Level"))
      {
        m_state.nextLevel(bonus, malus);
        return std::make_unique<LevelScreen>(getWindow(), m_state);
      }
    }
    nextLevelButtonSpacing = (ImGui::GetWindowContentRegionWidth() - ImGui::GetItemRectSize().x) / 2.f;

    ImGui::SetWindowPos({(getWindow().getSize().x - ImGui::GetWindowWidth()) / 2,
      (getWindow().getSize().y - ImGui::GetWindowHeight()) / 2.f});
    ImGui::End();

    getWindow().clear(sf::Color::Black);
    getWindow().draw(m_backgroundSprite);
    ImGui::SFML::Render(getWindow());
    getWindow().display();
  }

  return {};
}

}
