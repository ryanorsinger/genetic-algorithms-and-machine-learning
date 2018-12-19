#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include <catch.hpp> //https://github.com/philsquared/Catch.git

//first parametrer is test name
//second is a tag for filtering, like
//UnitTests.exe [Simples]

#include "../Lib/Particle.h"
#include "../Lib/PriceSimulation.h"

#include <random>


TEST_CASE("Test that random numbers in have mean 0", "[Samples]")
{
  std::mt19937 engine;
  std::normal_distribution<> normal_dist;

  REQUIRE(normal_dist.mean() == 0.0);
}

TEST_CASE("Test that random numbers in have standard deviation 1", "[Samples]")
{
  std::mt19937 engine;
  std::normal_distribution<> normal_dist;

  REQUIRE(normal_dist.stddev() == 1.0);
}

TEST_CASE("A particle with breakout can go through bag edge", "[BM]")
{
  float width = 10;
  float height = 10;
  Diffuse::Particle p(0.0f, 0.0f, 0.0f, width, height, true);
  p.Move(width + 1, 0);
  REQUIRE(p.X() > width);
}

TEST_CASE("A particle without breakout cannot go through bag edge", "[BM]")
{
  float width = 10;
  float height = 10;
  Diffuse::Particle p(0.0f, 0.0f, 0.0f, width, height, false);
  p.Move(width + 1, 0);
  REQUIRE(p.X() <= width);
}


TEST_CASE("A particle with breakout cannot go through bag bottom", "[BM]")
{
  float width = 10;
  float height = 10;
  Diffuse::Particle p(0.0f, 0.0f, 0.0f, width, height, true);
  p.Move(0, height+1);
  REQUIRE(p.Y() <= height);
}

TEST_CASE("A particle without breakout cannot go through bag bottom", "[BM]")
{
  float width = 10;
  float height = 10;
  Diffuse::Particle p(0.0f, 0.0f, 0.0f, width, height, false);
  p.Move(0, height + 1);
  REQUIRE(p.Y() <= height);
}

TEST_CASE("A stock price simulation starting at 0 remains at 0", "[Property]")
{
  const double start_price = 0.0;
  const double dt    = 0.1;//or whatever
  const unsigned int seed = 1;//or whatever
  Diffuse::PriceSimulation price(start_price, 0.3, 0.2, dt, seed);

  REQUIRE(price.Next() == 0.0);
}

TEST_CASE("Test that dt parameter gets set to requested value", "[ParseArgs]")
{
  int sims = 1;
  double dt = 0.5;
  double drift = 0.2;
  double vol = 0.3;
  double jump = 0.0;
  double mean_jump_per_unit_time = 0.1;
  
  int argc = 3;
  double expected_value = 0.400000006;
  const char *argv[] = {"prog name", "sims", "0.4"};
  dt = get_value(argc, &argv[0], 2, dt);
  //REQUIRE(expected_value == dt);//more precision needed
//  Catch::toString
  REQUIRE(Catch::toString(expected_value) == Catch::toString(dt));
}

