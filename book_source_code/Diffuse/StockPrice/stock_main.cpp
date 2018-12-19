#include "../Lib/PriceSimulation.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>

//disable link warning... LNK4099
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

void action(const std::vector<std::vector<sf::Vertex>> & sims,
  float time,
  float height,
  std::string title)
{
  const float edge = 30.0f;
  const float lineWidth = 5.0f;
  const float width = 500.0f;
  const float x_scale = width/time;
  const auto bagColour = sf::Color(180, 120, 60);
  sf::RenderWindow window(
    sf::VideoMode(static_cast<int>(width + 2*edge),
    static_cast<int>(height + 2*edge)),
    title);

  size_t last = 1;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
        break;
    }

    window.clear();

    drawBag(window, lineWidth, edge, height, width, bagColour);

    last = std::min(++last, sims.begin()->size() - 1);
    for(const auto & points: sims)
    {
      bool out = false;
      for(size_t i=0; i < last; ++i)
      {
         auto scaled_start = sf::Vertex(
           sf::Vector2f(points[i].position.x * x_scale + edge,
           height - points[i].position.y),
           sf::Color::White);
        auto scaled_point = sf::Vertex(
           sf::Vector2f(points[i+1].position.x * x_scale + edge,
           height - points[i+1].position.y),
           sf::Color::White);
        sf::Vertex line[] = {scaled_start, scaled_point};
        window.draw(line, 2, sf::Lines);
      }
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

std::vector<sf::Vertex> price_demo(unsigned int seed,
  double drift,
  double vol,
  double time,
  double dt,
  double jump,
  double mean_jump_per_unit_time)
{
  const double start_price = 50.0;
  Diffuse::PriceSimulation price(start_price, 
    drift,
    vol,
    dt,
    seed,
    jump,
    mean_jump_per_unit_time);

  std::vector<sf::Vertex> points;
  const int count = static_cast<int>(time/dt);
  for(int i=0; i <= count+1; ++i)
  {
    auto point = sf::Vector2f(static_cast<float>(i*dt),
      static_cast<float>(price.Next()));
    points.push_back(point);
  }
  return points;
}


int main(int argc, char ** argv)
{
  int sims = 1;
  double dt = 0.5;
  double drift = 0.2;
  double vol = 0.3;
  double jump = 0.0;
  double mean_jump_per_unit_time = 0.1;

  if (argc == 1)
  {
    std::cout << "usage:\n\tsims = " << sims << "\tdt = " << dt << "\tdrift = " << drift << "\tvol = " << vol
          << "\n\tjump = " << jump << "\tjump probability = " << mean_jump_per_unit_time << "\n";
    return -1;
  }


  if (argc >= 2)
  {
    try
    {
      sims = std::stoi(argv[1]);
      if (sims < 1)
      {
        std::cout << "sims must be positive.\n";
        return 1;
      }
    }
    catch(const std::exception &)
    {
      //I feel bad...
    }
  }
  dt = get_value(argc, const_cast<const char **>(argv), 2, dt);
  drift = get_value(argc, const_cast<const char **>(argv), 3, dt);
  vol = get_value(argc, const_cast<const char **>(argv), 4, dt);
  jump = get_value(argc, const_cast<const char **>(argv), 5, dt);
  if (argc >= 7)
  {
    try
    {
      //not jump prob
      mean_jump_per_unit_time = std::stof(argv[6]);
      if (mean_jump_per_unit_time == 0.0)
      {
        //ok, bad
        mean_jump_per_unit_time = 1;
        jump = 0;
        if (jump != 0.0)
          std::cout << "WARNING - work round jump\n";
      }
    }
    catch(const std::exception &)
    {
      //I feel bad...
    }
  }

  std::vector<std::vector<sf::Vertex>> allPoints;
  float time = 4.0f;
  float final_height = 0.0;
  int out = 0;
  const float height = 400.0f;
  using std::chrono::high_resolution_clock;
  for (int i=0; i<sims; ++i)
  {
    unsigned int seed = 
      static_cast<unsigned int>(
      high_resolution_clock::now().
      time_since_epoch().count()
      + i
    );
    auto sim = price_demo(seed,
      drift,
      vol,
      time,
      dt,
      jump,
      mean_jump_per_unit_time);
    allPoints.push_back(sim);
    auto price = sim.back().position.y;
    final_height += price;
    if (price > height)
      ++out;
  }
  std::cout << "average " << final_height/sims << "\nnumber out " << out << '\n';;

  std::stringstream title;
  title << "Stock prices. sims = " << sims << ", dt = " << dt << ", drift " << drift << ", vol " << vol << ", dt " << dt << ", jump size " << jump << ", mean jumps " << mean_jump_per_unit_time;
  action(allPoints, time, height, title.str());
}
