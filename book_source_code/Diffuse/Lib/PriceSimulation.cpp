#include "PriceSimulation.h"

#include <string>
using namespace Diffuse;

PriceSimulation::PriceSimulation(double price,
    double drift,
    double vol,
    double dt,
    unsigned int seed,
    double jump,
    double mean_jump_per_unit_time)
  :
  price(price),
  drift(drift),
  vol(vol),
  dt(dt),
  engine(seed),
  jump(jump),
  poisson_dist(mean_jump_per_unit_time * dt)
{
}

double PriceSimulation::Next()
{
  double dW = normal_dist(engine); 
  double dn = poisson_dist(engine); 
  double increment  = drift * dt
    + vol * sqrt(dt) * dW 
    + jump * dn; 
  price += price * increment;
  return price;
}

double get_value(int argc, const char **argv, int pos, double value)
{
  if (argc >= pos+1)
  {
    try
    {
      value = std::stof(argv[pos]);
    }
    catch(const std::exception &)
    {
      //I feel bad...
    }
  }
  return value;
}

