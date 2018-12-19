#include "../Bees/Bee.h"

#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

sf::CircleShape bee_shape(float size, BeeColony::Role role)
{
  sf::CircleShape shape(size);
  switch (role)
  {
    case BeeColony::Role::Worker:
    {
      shape.setPointCount(20);
      shape.setFillColor(sf::Color::Yellow);
    }
    break;
    case BeeColony::Role::Inactive:
    {
      shape.setPointCount(3);
      shape.setFillColor(sf::Color::Cyan);
    }
    break;
    case BeeColony::Role::Scout:
    {
      shape.setPointCount(5);
      shape.setFillColor(sf::Color::Magenta);
    }
    break;
  }
  return shape;
}

void draw_bees(const std::vector<BeeColony::Bee> & bees,
  sf::RenderWindow & window,
  float size,
  float edge,
  float width,
  float height)
{
  for(const auto & bee : bees)
  {
    sf::CircleShape shape = bee_shape(size, bee.get_role());

    float x = static_cast<float>(edge + size + bee.get_pos().x);
    if (x > edge + width - 2*size)
      x = edge + width - 2*size;
    if (x < edge + 2*size)
      x = edge + 2*size;
    float y = height - 2 * size - static_cast<float>(bee.get_pos().y);
    shape.setPosition(x, y);
    window.draw(shape);
  }
}

void draw_bag(sf::RenderWindow & window, float lineWidth, float edge,
	float height, float width, sf::Color bagColor)
{
	sf::RectangleShape  left(
		sf::Vector2f(lineWidth, height)
	);
	left.setFillColor(bagColor);
	left.setPosition(edge, edge);
	window.draw(left);

	sf::RectangleShape  right(
		sf::Vector2f(lineWidth, height)
	);
	right.setFillColor(bagColor);
	right.setPosition(edge + width, edge);
	window.draw(right);

	sf::RectangleShape  base(
		sf::Vector2f(width + lineWidth, lineWidth)
	);
	base.setFillColor(bagColor);
	base.setPosition(edge, edge + height);
	window.draw(base);
}

void action(BeeColony::Hive hive,
  float width,
  float edge,
  float bee_size = 10.0f)
{
  const float lineWidth = 10.0f;
  const float height = 400.0f;
  const auto bagColor = sf::Color(180, 120, 60);
  sf::RenderWindow window(
    sf::VideoMode(
      static_cast<int>(width + 2*edge),
      static_cast<int>(height + 2*edge)
    ),
    "ABC");

  bool paused = false;
  bool swarmed = false;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed)
        paused = !paused;
    }

    window.clear();
    draw_bag(window, lineWidth, edge, height, width, bagColor);

    if (!paused)
    {
      hive.update_bees();
      if (!swarmed && should_swarm(hive.get_bees(), height + bee_size))
      {
        hive.swarm();
        swarmed = true;
      }
    }
    draw_bees(hive.get_bees(), window, bee_size, edge, width, height + edge);
    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

int get_value(int argc, const char **argv, int pos, int value)
{
  if (argc >= pos+1)
  {
    try
    {
      value = std::stoi(argv[pos]);
    }
    catch(const std::exception &)
    {
      //*cough*
    }
  }
  return value;
}


using namespace BeeColony;

int main(int argc, const char** argv)
{
  try
  {
    int number_workers = get_value(argc, argv, 1, 10);
    int number_inactive = get_value(argc, argv, 2, 5);
    int number_scout = get_value(argc, argv, 3, 3);
    const float width = 500.0f;
    const float edge = 150.0f;
    const float bee_size = 10.0f;
    const Coordinate food {edge + width - 2 * bee_size, 0.0f};
    Hive hive(number_workers,
        number_inactive, number_scout, { width / 2, 10.0f }, food, bee_size, 100);
    action(hive, width, edge, bee_size);
  }
  catch(const std::exception & e)
  {
    std::cout << e.what() << '\n';
  }
}
