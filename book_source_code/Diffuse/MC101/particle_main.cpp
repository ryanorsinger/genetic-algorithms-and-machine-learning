#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>

#include "../Lib/Air.h"
#include "../Lib/Particle.h"

#include <SFML/Graphics.hpp>

void drawBag(sf::RenderWindow  & window, 
  float lineWidth,
  float edge,
  float height,
  float width,
  sf::Color bagColour)
{
  sf::RectangleShape  left(sf::Vector2f(lineWidth, height));
  left.setFillColor(bagColour);
  left.setPosition(edge, edge);

  sf::RectangleShape  right(sf::Vector2f(lineWidth, height));
  right.setFillColor(bagColour);
  right.setPosition(edge + width, edge);

  sf::RectangleShape  base(sf::Vector2f(width + lineWidth, lineWidth));
  base.setFillColor(bagColour);
  base.setPosition(edge, edge + height);

  window.draw(left);
  window.draw(right);
  window.draw(base);
}

std::vector<Diffuse::Particle> createParticles(size_t count,
  float start_x,
  float start_y,
  float lineWidth,
  float edge,
  float height,
  float width,
  bool breakout)
{
  std::vector<Diffuse::Particle> particles;
  for (size_t i = 0; i < count; ++i)
  {
    particles.emplace_back(
      start_x,
      start_y,
      edge + lineWidth,
      edge / 2 + width - 2 * lineWidth,
      edge / 2 + height - 2 * lineWidth,
      breakout
      );
  }
  return particles;
}

void action(size_t count, float step, bool breakout)
{
  std::stringstream title;
  title << "2D Brownian motion " << count << ", breakout " << breakout;

  const float height = 500.0f;
  const float width = 500.0f;
  const float edge = 50.0f;
  const float lineWidth = 5.0f;
  const auto bagColour = sf::Color(180, 120, 60);

  int max_x = static_cast<int>(width + edge);
  int max_y = static_cast<int>(height + edge);
  sf::RenderWindow window(sf::VideoMode(max_x, max_y),
              title.str());

  std::vector<Diffuse::Particle> particles = 
    createParticles(count, max_x/2.0f, max_y/2.0f,
                    lineWidth, edge,
                    height, width, breakout);

  std::random_device rd;
  Diffuse::Air air(step, rd());

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

    drawBag(window, lineWidth, edge/2, height, width, bagColour);

    sf::CircleShape shape(lineWidth);
    shape.setFillColor(sf::Color::Green);
    for(auto & particle: particles) 
    {
      if (!paused)
        particle.Move(air.Bump(), air.Bump());
      shape.setPosition(particle.X(), particle.Y());
      window.draw(shape);
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main(int argc, char ** argv)
{
  int particles = 25;
  bool breakout = true;
  if (argc >= 2)
  {
    try
    {
      particles = std::stoi(argv[1]);

      if (particles < 1)
      {
        std::cout << "Particles must be positive.\n";
        return 0;
      }
      if (argc >= 3)
      {
        breakout = std::atoi(argv[2]) != 0;
      }
    }
    catch(const std::exception &)
    {
      //I feel bad...
    }
  }

  const float step = 7.5f;
  action(particles, step, breakout);
}
