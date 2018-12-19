#include "../GACA/GACA.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <thread>

#include <SFML/Graphics.hpp>

using namespace GACA;

void drawBag(sf::RenderWindow & window,
  float line_width,
  float edge,
  float height,
  float width,
  float cell_size,
  sf::Color bagColor)
{
  sf::RectangleShape  left(sf::Vector2f(line_width, height));
  left.setFillColor(bagColor);
  left.setPosition(edge - line_width, edge + line_width);
  window.draw(left);

  sf::RectangleShape  right(sf::Vector2f(line_width, height));
  right.setFillColor(bagColor);
  right.setPosition(edge + width, edge + line_width);
  window.draw(right);

  sf::RectangleShape  base(sf::Vector2f(width + line_width + cell_size, line_width));
  base.setFillColor(bagColor);
  base.setPosition(edge - cell_size, edge + height + cell_size);
  window.draw(base);
}

void draw(World & world)
{
  const size_t edge = 15;
  const float cell_size = 10.0f;
  const float width = world.Width() * 2*cell_size;
  const float margin = edge * cell_size;
  const float line_width = 10.0f;
  const float height = (world.Height() + edge)* cell_size;
  const float bag_height = world.Height() * cell_size;
  const auto bagColor = sf::Color(180, 120, 60);

  const int window_x = static_cast<int>(width + 2* margin);
  const int window_y = static_cast<int>(height + margin);
  sf::RenderWindow window(sf::VideoMode(window_x, window_y), "Dream!");

  bool paused = false;
  size_t row = 1; 
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
      bagColor);
    for(size_t y=0; y<row; ++y) 
    {
      for(size_t x=0; x<world.Width(); ++x)
      {
        if(world.Alive(x, y))
        {
          sf::CircleShape shape(5);
          shape.setFillColor(sf::Color::Cyan);
          shape.setPosition(x * 2*cell_size + margin, height - y * cell_size); 
          window.draw(shape);
        }
      }
    }

    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if(!paused && (row < (world.Height() + edge/2.0)))
      ++row; 
  }
}

Population start(const GACA::Rule & rule,
  size_t size,
  size_t updates)
{
  std::random_device rd;
  Population population;
  RowGenerator cell_generator(rd());
  for (size_t i = 0; i<size; ++i)
    population.emplace_back(rule, cell_generator.generate(), updates);
  return population;
}


GACA::World ga_ca(const GACA::Rule & rule,
  size_t size,
  double rate,
  size_t epochs,
  size_t updates,
  bool middle,
  bool target)
{
  Population population = start(rule, size, updates);

  std::random_device rd;
  Mutation mutation(rd(), rate);
  Crossover crossover(rd(), size, rule, updates, middle, target);
  for(size_t epoch = 0; epoch < epochs; ++epoch)
  {
    population = crossover(population);
    for(auto & world : population)
      world.Reset(mutation(world.StartState()));

    const World & curr_best_world = best(population, target);

    auto alive = fitness(curr_best_world.State(), target);
    std::cout << epoch << " : " << alive << '\n';
  }
  const World & best_world = best(population, target);
  std::cout << "Final best fitness "
            << fitness(best_world.State(), target) << '\n';
  return best_world;
}

std::shared_ptr<Rule> make_rule(bool jit_rule, bool eca)
{
    if (jit_rule)
    {
        std::random_device rd;
        return std::make_shared<DreamRule>(rd());
    }
    else if (eca)
    {
        std::random_device rd;
        std::default_random_engine gen(rd());
        std::uniform_int_distribution<size_t> uniform_dist(0, 255);
        size_t number = uniform_dist(gen);
        std::cout << "eca rule " << number << '\n';
        return std::make_shared<ECARule>(number);
    }
  else
  {
      return std::make_shared<StaticRule>();
  }
}

void gaca_visual(size_t population, bool jit_rule, bool eca, double rate, size_t epochs, size_t height, bool display, bool middle, bool target)
{
  using namespace GACA;
  Row row;
  std::shared_ptr<Rule> rule = make_rule(jit_rule, eca);

  GACA::World world = ga_ca(*rule, population, rate, epochs, height, middle, target);
  std::cout << "Row size " << std::tuple_size<Row>::value << '\n';
  std::cout << "fitness of start cell " << fitness(row, target)
        << " fitness of final cell " << fitness(world.State(), target) << '\n';
  if (display)
    draw(world);
}

void eca_display(size_t number, double rate, size_t height)
{
  using namespace GACA;
  std::shared_ptr<Rule> rule = std::make_shared<ECARule>(number);
  Row cell;
  cell.fill(false);
  cell[cell.size()/2]=true;
  World world(*rule, cell, height);
  draw(world);
}

int main(int argc, char** argv)
{
  try
  {
    int arg = 1;

    int population = 25;
    double rate = 0.5;
    bool jit_rule = false;
    bool eca = false;
    int epochs = 20;
    int height = 32;
    bool middle = false;
    bool target = true;
    bool display = true;
    int eca_rule = -1;

    while(arg < argc)
    {
      if(std::string("eca_rule=")==std::string(argv[arg]).substr(0, 9))
      {
        auto value = std::string(argv[arg]).substr(9);
        eca_rule = std::stoi(value);
        std::cout << "eca_rule " << eca_rule << "\n";
      }
      else if(std::string("population=")==std::string(argv[arg]).substr(0, 11))
      {
        auto value = std::string(argv[arg]).substr(11);
        population = std::stoi(value);
        std::cout << "population " << population << "\n";
      }
      else if (std::string("rate=") == std::string(argv[arg]).substr(0, 5))
      {
        auto value = std::string(argv[arg]).substr(5);
        rate = std::stof(value);
        std::cout << "rate " << rate << "\n";
      }
      else if (std::string("jit_rule") == argv[arg])
      {
          jit_rule = true;
      }
      else if (std::string("eca") == argv[arg])
      {
          eca = true;
      }
      else if (std::string("epochs=") == std::string(argv[arg]).substr(0, 7))
      {
        auto value = std::string(argv[arg]).substr(7);
        epochs = std::stoi(value);
        std::cout << "epochs " << epochs << "\n";
      }
      else if (std::string("display=") == std::string(argv[arg]).substr(0, 8))
      {
          auto value = std::string(argv[arg]).substr(8);
          std::istringstream(value) >> std::boolalpha >> display;
          std::cout << "display " << display << "\n";
      }
      else if (std::string("target=") == std::string(argv[arg]).substr(0, 7))
      {
          auto value = std::string(argv[arg]).substr(7);
          std::istringstream(value) >> std::boolalpha >> target;
          std::cout << "target " << target << "\n";
      }
      else if (std::string("middle=") == std::string(argv[arg]).substr(0, 7))
      {
          auto value = std::string(argv[arg]).substr(7);
          std::istringstream(value) >> std::boolalpha >> middle;
          std::cout << "middle " << middle << "\n";
      }
      else if (std::string("height=") == std::string(argv[arg]).substr(0, 7))
      {
        auto value = std::string(argv[arg]).substr(7);
        height = stoi(value);
        std::cout << "height " << height << "\n";
      }
      else
      {
        std::cout << "Unused parameter:" << argv[arg];
        return -1;
      }
      ++arg;
    }
    if (eca_rule > -1)
    {
        eca_display(static_cast<size_t>(eca_rule), rate, height);
    }
    else
    {
        gaca_visual(population, jit_rule, eca, rate, epochs, height, display, middle, target);
    }
  }
  catch(const std::exception & e)
  {
    std::cerr << "ERROR:\n" << e.what() << '\n';
  }
}
