#ifndef DAEDALUS_GUI_HPP
#define DAEDALUS_GUI_HPP

#include "resources.hpp"

#include <SFML/System/Sleep.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <imgui.h>

#include <string>

namespace daedalus::Gui
{

template <typename... T>
bool SoundButton(T&&... p_args)
{
  if (ImGui::Button(std::forward<T>(p_args)...))
  {
    sf::SoundBuffer buffer;
    buffer.loadFromFile(getResource("audio/tiny-button-push.wav"));

    sf::Sound sound(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing)
    {
    }

    return true;
  }
  return false;
}

}

#endif
