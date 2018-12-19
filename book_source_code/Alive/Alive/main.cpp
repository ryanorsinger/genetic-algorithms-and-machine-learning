#include "../GameOfLife/GoL.h"

#include <iostream>
#include <random>
#include <thread>

#include <SFML/Graphics.hpp>

using namespace GoL;

void drawBag(sf::RenderWindow & window,
  float lineWidth,
  float edge,
  float height,
  float width,
  float cell_size,
  sf::Color bagColour)
{
  sf::RectangleShape  left(sf::Vector2f(lineWidth, height));
  left.setFillColor(bagColour);
  left.setPosition(edge - lineWidth, edge + lineWidth);
  window.draw(left);

  sf::RectangleShape  right(sf::Vector2f(lineWidth, height));
  right.setFillColor(bagColour);
  right.setPosition(edge + width, edge + lineWidth);
  window.draw(right);

  sf::RectangleShape  base(sf::Vector2f(width + lineWidth + cell_size, lineWidth));
  base.setFillColor(bagColour);
  base.setPosition(edge - cell_size, height + edge + cell_size);
  window.draw(base);
}

void draw_world(const World & world,
                float cell_size,
                float height,
                size_t edge,
                sf::RenderWindow & window)
{
  for (size_t y = 0; y<world.Height(); ++y)
  {
    for (size_t x = 0; x<world.Width(); ++x)
    {
      if (world.Alive(x, y))
      {
        sf::CircleShape shape(5);
        shape.setFillColor(sf::Color::Cyan);
        shape.setPosition((x + edge) * cell_size, height - y * cell_size);

        window.draw(shape);
      }
    }
  }
}

void draw(World & world, size_t edge)
{
  const float cell_size = 10.0f;
  const float width = world.Width() * cell_size;
  const float margin = edge * cell_size;
  const float line_width = 10.0f;
  const float height = world.Height() * cell_size;
  const float bag_height = (world.Height() - edge) * cell_size;
  const auto bag_color = sf::Color(180, 120, 60);

  sf::RenderWindow window(
    sf::VideoMode(
      static_cast<int>(width + 2 * margin),
      static_cast<int>(height + margin)),
    "Game of Life");

  bool paused = false;
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
    drawBag(window,
        line_width,
        margin,
        bag_height,
        width,
        cell_size,
        bag_color);

    draw_world(world, cell_size, height, edge, window);

    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if(!paused)
    {
      world.Update();
    }
  }
}

void game_of_life(bool wrap, bool glide, size_t bag_x, size_t bag_y)
{
  const size_t edge = 10;

  const size_t world_x = bag_x;
  const size_t world_y = bag_y + edge;

  size_t sparks = glide ? 0 : 800;
  World world = random_setup(bag_x, bag_y, world_x, world_y, sparks, wrap);
  if (glide)
  {
    inverted_glider(world, bag_x/2, bag_y/2);
  }
  draw(world, edge);
}

int main(int argc, char** argv)
{
  try
  {
    bool wrap = false;
    bool glider = false;
    size_t height = 40;
    size_t width = 50;
    int arg = 1;
    while(arg < argc)
    {
      if (std::string("wrap") == argv[arg])
      {
        wrap = true;
        std::cout << "Wrap fully\n";
      }
      else if (std::string("glider") == argv[arg])
      {
        glider = true;
        std::cout << "Glider\n";
      }
      else if (std::string("height=") == std::string(argv[arg]).substr(0, 7))
      {
          auto value = std::string(argv[arg]).substr(7);
          height = stoi(value);
          std::cout << "height " << height << "\n";
      }
      else if (std::string("width=") == std::string(argv[arg]).substr(0, 6))
      {
          auto value = std::string(argv[arg]).substr(6);
          width = stoi(value);
          std::cout << "width " << width << "\n";
      }
      else
      {
        std::cout << "Unused parameter:" << argv[arg];
        return -1;
      }
      ++arg;
    }
    game_of_life(wrap, glider, width, height);
  }
  catch(const std::exception & e)
  {
    std::cerr << "ERROR:\n" << e.what() << '\n';
  }
}
