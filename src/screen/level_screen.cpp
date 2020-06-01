#include "level_screen.hpp"

#include "generator.hpp"
#include "next_level_screen.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <random>

namespace daedalus
{

LevelScreen::LevelScreen(sf::RenderWindow& p_window, float p_allowedTime,
  std::pair<std::size_t, std::size_t> p_size)
  : Screen(p_window)
  , m_maze([p_size]()
    {
      std::mt19937 rng(std::random_device{}());
      std::uniform_int_distribution<std::size_t> sizeDist(p_size.first, p_size.second);
      return Generator{sizeDist(rng), sizeDist(rng)}.primMaze();
    }())
  , m_allowedTime(p_allowedTime)
  , m_size(p_size)
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
            m_maze.movePlayer(daedalus::Direction::North);
            break;
          }
          case sf::Keyboard::Down:
          {
            m_maze.movePlayer(daedalus::Direction::South);
            break;
          }
          case sf::Keyboard::Right:
          {
            m_maze.movePlayer(daedalus::Direction::East);
            break;
          }
          case sf::Keyboard::Left:
          {
            m_maze.movePlayer(daedalus::Direction::West);
            break;
          }
          case sf::Keyboard::Space:
          {
            if (m_maze.hasWon())
            {
              {
                daedalus::NextLevelScreen nextLevelScreen(getWindow(), m_maze.hasBonus());
                nextLevelScreen.run();
              }

              if (m_maze.hasBonus())
              {
                m_allowedTime += 0.1;
              }
              return std::make_unique<LevelScreen>(getWindow(), m_allowedTime,
                std::pair<std::size_t, std::size_t>{m_size.first + 1, m_size.second + 1});
            }
            break;
          }
          default:
            break;
        }
      }
    }

    getWindow().clear();

    auto const maxDelta = 0.01f * deltaClock.getElapsedTime().asSeconds();
    auto deltaCenter = m_maze.getVisibleCenter() - getWindow().getView().getCenter();
    deltaCenter = {std::clamp(deltaCenter.x, -maxDelta, maxDelta), std::clamp(deltaCenter.y, -maxDelta, maxDelta)};
    centerView(getWindow(), getWindow().getView().getCenter() + deltaCenter);
    getWindow().draw(m_maze);

    drawHUD(clock);

    getWindow().display();
  }

  return {};
}

void LevelScreen::drawHUD(sf::Clock const& p_clock)
{
  constexpr int OFFSET = 10;

  auto const timeRatio =
    p_clock.getElapsedTime().asSeconds() / (m_allowedTime * m_maze.getWidth() * m_maze.getHeight());
  if (timeRatio > 1)
  {
    getWindow().close();
  }

  auto saveView = getWindow().getView();
  getWindow().setView(getWindow().getDefaultView());

  constexpr int WIDTH = 25;
  auto const height = getWindow().getSize().y - 2.0f * OFFSET;

  sf::RectangleShape timer({WIDTH, height});
  timer.setPosition(getWindow().getSize().x - WIDTH - OFFSET, OFFSET);

  timer.setFillColor({130, 200, 50});
  getWindow().draw(timer);

  if (timeRatio < 0.33)
  {
    timer.setFillColor({255, 230, 120});
  }
  else if (timeRatio < 0.66)
  {
    timer.setFillColor({200, 100, 50});
  }
  else
  {
    timer.setFillColor({240, 80, 90});
  }
  timer.setSize({WIDTH , height * timeRatio});
  getWindow().draw(timer);

  sf::RectangleShape itemStatus({daedalus::Cell::PIXELS, daedalus::Cell::PIXELS});
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
