#include "level_screen.hpp"

#include "generator.hpp"
#include "next_level_screen.hpp"
#include "lost_screen.hpp"
#include "resources.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
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
  , m_maze(Generator{m_state.newWidth(), m_state.newHeight()}.primMaze(p_state))
{
}

std::unique_ptr<Screen> LevelScreen::run()
{
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
              return std::make_unique<NextLevelScreen>(getWindow(), createBackground(), m_state);
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
      return std::make_unique<LostScreen>(getWindow(), createBackground(), m_state.currentLevel());
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

  constexpr int TIMER_WIDTH = 25;
  auto const height = getWindow().getSize().y - 2.0f * OFFSET;
  sf::RectangleShape timer({TIMER_WIDTH, height});
  timer.setPosition(getWindow().getSize().x - TIMER_WIDTH - OFFSET, OFFSET);

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
  timer.setSize({TIMER_WIDTH , height * p_timeRatio});
  getWindow().draw(timer);

  // Door
  auto door = m_maze.getDoorIndicator();
  door.setPosition({OFFSET, OFFSET});
  getWindow().draw(door);
  // Bonus
  auto bonus = m_maze.getBonusIndicator();
  bonus.setPosition(Cell::PIXELS + 2 * OFFSET, OFFSET);
  getWindow().draw(bonus);

  getWindow().setView(saveView);
}

void LevelScreen::centerView(sf::RenderTarget& p_target, sf::Vector2f p_center)
{
  auto view = p_target.getView();
  view.setCenter(p_center);
  p_target.setView(view);
}

sf::Texture LevelScreen::createBackground()
{
  auto windowSize = getWindow().getSize();

  sf::RenderTexture render;
  render.create(windowSize.x, windowSize.y);

  centerView(render, m_maze.getVisibleCenter());
  render.draw(m_maze);

  sf::RectangleShape blur(sf::Vector2f(windowSize.x, windowSize.y));
  blur.setFillColor({255, 255, 255, 128});
  render.setView(render.getDefaultView());
  render.draw(blur);

  render.display();
  return render.getTexture();
}

}
