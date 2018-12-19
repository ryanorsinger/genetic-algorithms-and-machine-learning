/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef PRICE_SIMULATION_INCLUDED
#define PRICE_SIMULATION_INCLUDED

#include <chrono>
#include <random>

namespace Diffuse
{
  class PriceSimulation
  {
  public:
    PriceSimulation(double price,
      double drift, 
      double vol,
      double dt,
      unsigned int seed = 
        std::chrono::high_resolution_clock::now().
                       time_since_epoch().count(),
      double jump = 0.0,
      double mean_jump_per_unit_time = 0.1);

    double Next();

  private:
    double price;
    double drift;
    double vol;
    double dt;
    double jump;

    std::mt19937 engine;
    std::normal_distribution<> normal_dist;
    std::poisson_distribution<> poisson_dist;
  };
}

double get_value(int argc, const char **argv, int pos, double value);

#endif
