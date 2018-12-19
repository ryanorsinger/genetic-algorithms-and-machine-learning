/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef BEE_INCLUDED
#define BEE_INCLUDED

#include <random>
#include <vector>

namespace BeeColony
{

  enum class Role 
  {
    Worker,
    Inactive,
    Scout
  };

  class Hive;

  struct Coordinate
  {
    double x;
    double y;
  };

  //overall quality of solution - the higher the better
  // - leave y as big is good then do height minus to draw it
  inline double quality(Coordinate position)
  {
    return position.y;
  }

  Hive make_hive(int number_workers,
    int number_inactive,
    int number_scout,
    Coordinate start_pos, Coordinate food, float bee_size, int steps);

class Bee
{
public:
  explicit Bee(Role role,
    Coordinate position = { 0.0, 0.0 },
    Coordinate food = { 0.0, 0.0 },
    double buzz = 5.0)
    : role(role),
    position(position),
    home(position),
    buzz(buzz),
    food(food)
  {
  }

  Role get_role() const { return role; }

  void communicate(Role new_role, Coordinate new_food);
  void scout(double x_move, double y_move);
  void work(double x_move, double y_move);
  void go_home();
  
  bool is_home() const
  {
    return (position.y > home.y - buzz)
           && (position.y < home.y + buzz) 
           && (position.x > home.x - buzz)
           && (position.x < home.x + buzz);
  }

  void waggle(double jiggle);
  Coordinate get_pos() const { return position; }
  Coordinate get_food() const { return food; }

  void move_home(Coordinate new_home) 
  {
    home = new_home;
  }

private:
  Role role;
  Coordinate position;
  Coordinate home;
  Coordinate food;
  const double buzz;
};

class Hive
{
public: 
  Hive(int number_workers,
    int number_inactive,
    int number_scout,
    Coordinate start_pos, Coordinate food, float buzz, int steps);

  std::vector<BeeColony::Bee> get_bees() const 
  {
    return bees;
  }

  void update_bees();
  void swarm();
  bool all_home();

private:
  std::vector<Bee> bees;
  const size_t steps;
  size_t step;
  std::mt19937 engine;
  std::normal_distribution<double> normal_dist;
  std::uniform_int_distribution<> uniform_dist;

  void waggle_dance();
  void explore();
};

  bool should_swarm(const std::vector<BeeColony::Bee> & bees, double target);
  void move(Coordinate & from, const Coordinate to, double step);
}

#endif
