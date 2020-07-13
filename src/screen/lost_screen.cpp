#include "lost_screen.hpp"

#include "start_screen.hpp"
#include "resources.hpp"
#include "gui.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

namespace daedalus
{

LostScreen::LostScreen(sf::RenderWindow& p_window, sf::Texture p_background, int p_lostLevel)
  : Screen(p_window)
  , m_background(p_background)
  , m_backgroundSprite(m_background)
  , m_lostLevel(p_lostLevel)
{
}

std::unique_ptr<Screen> LostScreen::run()
{
  float okButtonSpacing{};

  sf::SoundBuffer tickTockBuffer;
  tickTockBuffer.loadFromFile(getResource("audio/sad-trombone.wav"));
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

    ImGui::Begin("lost_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    ImGui::Text("You lost at level %d :-(", m_lostLevel);

    ImGui::Dummy({okButtonSpacing, -1});
    ImGui::SameLine();
    if (Gui::SoundButton("OK"))
    {
      return std::make_unique<StartScreen>(getWindow());
    }
    okButtonSpacing = (ImGui::GetWindowContentRegionWidth() - ImGui::GetItemRectSize().x) / 2.f;

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
