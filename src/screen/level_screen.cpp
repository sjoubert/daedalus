#include "level_screen.hpp"

#include "generator.hpp"
#include "next_level_screen.hpp"
#include "lost_screen.hpp"
#include "resources.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <random>

namespace daedalus
{

LevelScreen::LevelScreen(sf::RenderWindow& p_window, RunState p_state)
  : Screen(p_window)
  , m_state(std::move(p_state))
  , m_maze(Generator{m_state.newWidth(), m_state.newHeight()}.primMaze())
{
}

std::unique_ptr<Screen> LevelScreen::run()
{
  auto centerView = [](sf::RenderTarget& p_target, sf::Vector2f p_center)
  {
    auto view = p_target.getView();
    view.setCenter(p_center);
    p_target.setView(view);
  };
  centerView(getWindow(), m_maze.getVisibleCenter());

  sf::SoundBuffer tickTockBuffer;
  tickTockBuffer.loadFromFile(getResource("audio/tick-tock.wav"));
  sf::Sound tickTockSound(tickTockBuffer);
  tickTockSound.setLoop(true);
  auto const allowedTime = sf::seconds(m_state.getTimeFactor() * m_maze.getWidth() * m_maze.getHeight());

  sf::Clock clock;
  sf::Clock deltaClock;
  while (getWindow().isOpen())
  {
    sf::Event event;
    while (getWindow().pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        getWindow().close();
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        switch (event.key.code)
        {
          case sf::Keyboard::Up:
          {
            m_maze.movePlayer(Maze::Direction::North);
            break;
          }
          case sf::Keyboard::Down:
          {
            m_maze.movePlayer(Maze::Direction::South);
            break;
          }
          case sf::Keyboard::Right:
          {
            m_maze.movePlayer(Maze::Direction::East);
            break;
          }
          case sf::Keyboard::Left:
          {
            m_maze.movePlayer(Maze::Direction::West);
            break;
          }
          case sf::Keyboard::Space:
          {
            if (m_maze.hasWon())
            {
              if (m_maze.hasBonus())
              {
                m_state.addBonus();
              }
              return std::make_unique<NextLevelScreen>(getWindow(), m_state);
            }
            break;
          }
          default:
            break;
        }
      }
    }


    auto const timeRatio = clock.getElapsedTime() / allowedTime;
    if (timeRatio > 1)
    {
      return std::make_unique<LostScreen>(getWindow(), m_state.currentLevel());
    }
    auto const warningClock = (allowedTime - clock.getElapsedTime()) < sf::seconds(10) && timeRatio > 0.85;
    if (tickTockSound.getStatus() != sf::SoundSource::Playing && warningClock)
    {
      tickTockSound.play();
    }

    getWindow().clear();

    auto const maxDelta = Cell::PIXELS * deltaClock.restart().asSeconds();
    auto deltaCenter = m_maze.getVisibleCenter() - getWindow().getView().getCenter();
    deltaCenter = {std::clamp(deltaCenter.x, -maxDelta, maxDelta), std::clamp(deltaCenter.y, -maxDelta, maxDelta)};
    centerView(getWindow(), getWindow().getView().getCenter() + deltaCenter);
    getWindow().draw(m_maze);

    drawHUD(timeRatio);

    getWindow().display();
  }

  return {};
}

void LevelScreen::drawHUD(float p_timeRatio)
{
  auto saveView = getWindow().getView();
  getWindow().setView(getWindow().getDefaultView());

  constexpr int OFFSET = 10;
  constexpr int WIDTH = 25;
  auto const height = getWindow().getSize().y - 2.0f * OFFSET;

  sf::RectangleShape timer({WIDTH, height});
  timer.setPosition(getWindow().getSize().x - WIDTH - OFFSET, OFFSET);

  timer.setFillColor({130, 200, 50});
  getWindow().draw(timer);

  if (p_timeRatio < 0.33)
  {
    timer.setFillColor({255, 230, 120});
  }
  else if (p_timeRatio < 0.66)
  {
    timer.setFillColor({200, 100, 50});
  }
  else
  {
    timer.setFillColor({240, 80, 90});
  }
  timer.setSize({WIDTH , height * p_timeRatio});
  getWindow().draw(timer);

  sf::RectangleShape itemStatus({Cell::PIXELS, Cell::PIXELS});
  // Door
  itemStatus.setFillColor(m_maze.doorIsOpen() ? sf::Color::Green : sf::Color::Red);
  itemStatus.setPosition({OFFSET, OFFSET});
  getWindow().draw(itemStatus);
  // Bonus
  if (m_maze.hasBonus())
  {
    itemStatus.setFillColor(sf::Color::Yellow);
  }
  else
  {
    itemStatus.setFillColor(sf::Color::Transparent);
    itemStatus.setOutlineColor(sf::Color::Yellow);
    itemStatus.setOutlineThickness(2);
  }
  itemStatus.move(WIDTH + OFFSET, 0);
  getWindow().draw(itemStatus);

  getWindow().setView(saveView);
}

}
