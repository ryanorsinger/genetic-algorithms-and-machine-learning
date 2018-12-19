#include "quickcheck/quickcheck.hh" //http://software.legiasoft.com/git/quickcheck.git
#include "../Lib/PriceSimulation.h"
#include <numeric>
#include <stdexcept>
#include <vector>

using namespace quickcheck;

class ZeroVolPriceGenerator
{
public:
  ZeroVolPriceGenerator() :
    start_price_(0),
    drift_(0.1),
    dt_(1),
    sims_(1),
    price_(0, drift_, 0.0, 1)
  {
  }

  void reset(double start_price, double drift, double dt, int sims)
  {
    start_price_ = start_price;
    drift_ = drift;
    dt_ = dt;
    sims_ = sims;
    price_ = Diffuse::PriceSimulation(start_price_, drift_, 0.0, dt_, 1);
  }


  double StartPrice() const { return start_price_; }
  double Drift() const { return drift_; }
  double Dt() const { return dt_; }
  int Sims() const { return sims_; }

  std::vector<double> prices() const 
  {
    std::vector<double> prices;

    for(int i=0; i<sims_; ++i)
    {
      prices.push_back(price_.Next());
    }
    return prices;
  }

private:
  double start_price_;
  double drift_;
  double dt_;
  int sims_;
  mutable Diffuse::PriceSimulation price_;
};

std::ostream & operator << (std::ostream & os, const ZeroVolPriceGenerator & gen)
{
  os << "Start price " << gen.StartPrice() << ", drift " << gen.Drift() << ", dt " << gen.Dt() << ", sims " << gen.Sims() << '\n';
    for(const auto & price : gen.prices())
  {
    os << price << '\n';
  }
  return os;
}

void generate(size_t n, ZeroVolPriceGenerator & out)
{
  double price;
  double drift;
  double dt;
  int sims;
  generate(n, price);
  generate(n, drift);
  dt = 0.1;
  generate(n, sims);
  out.reset(price, drift, dt, sims);
}

class ZeroVolGivesDrift : public Property<ZeroVolPriceGenerator> {

  bool holdsFor(const ZeroVolPriceGenerator& gen)
  {
    std::vector<double> xs = gen.prices();
    double tot = 0;
    double previous = gen.StartPrice();
    for(const auto & p : xs)
    {
      tot += (p - previous)/previous;
      previous = p;
    }
    auto average = tot/xs.size();
    auto scaled = std::abs(average/gen.Dt() - gen.Drift())/gen.Drift();
    return scaled <= 1.0;
  }

  bool accepts(const ZeroVolPriceGenerator& gen)
  {
    return gen.Sims() >= 1 //is 0 ok?
      && gen.Dt() > 0.0
      && std::abs(gen.Drift()) < 50.0;
    //Beware - we can fall out of the range of a double:
      // 5.41822e+307, -143.251
      //-1.#INF, -1.#INF
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //-1.#IND, -1.#IND
      //Falsifiable after 300 tests for input:
      //  0: Start price 100, drift -143.251, dt 1, sims 151
    //but then it can't find anything suitable:
    //Arguments exhausted after 88 tests.

  }
};

class ZeroStartPriceGenerator
{
public:
  ZeroStartPriceGenerator() : price_(0.0, drift_, 0.0, 0.1) {}

  void reset(double drift, double dt, int sims, unsigned int seed)
  {
    drift_ = drift;
    dt_ = dt;
    sims_ = sims;
    seed_ = seed;
    price_ = Diffuse::PriceSimulation(0.0, drift_, 0.0, dt_, seed);
  }

  double Seed() const { return seed_; }
  double Drift() const { return drift_; }
  double Dt() const { return dt_; }
  int Sims() const { return sims_; }

  std::vector<double> prices() const 
  {
    std::vector<double> prices;
    for(int i=0; i<sims_; ++i)
      prices.push_back(price_.Next());
    return prices;
  }

private:
  double drift_;
  double dt_;
  int sims_;
  double seed_;
  mutable Diffuse::PriceSimulation price_;
};

std::ostream & operator << (std::ostream & os, const ZeroStartPriceGenerator & gen)
{
  os << "Seed " << gen.Seed() << ", drift " << gen.Drift() << ", dt " << gen.Dt() << ", sims " << gen.Sims() << '\n';
    for(const auto & price : gen.prices())
  {
    os << price << '\n';
  }
  return os;
}

void generate(size_t n, ZeroStartPriceGenerator & out)
{
  double drift, dt;
  int sims;
  unsigned int seed;
  generate(n, drift);
  generate(n, dt);
  generate(n, sims);
  generate(n, seed);
  if (dt < 0) dt *= -1;//filter out negatives
  out.reset(drift, dt, sims, seed);
}

class ZeroStartPriceGivesZero : public Property<ZeroStartPriceGenerator> {

  bool holdsFor(const ZeroStartPriceGenerator& gen)
  {
    std::vector<double> xs = gen.prices();
    for(const auto & p : xs)
      if (p != 0.0) return false;
    return true;
  }

  bool accepts(const ZeroStartPriceGenerator& gen)
  {
    return gen.Dt() > 0.0;
  }
};

class ZeroDriftGenerator
{
public:
  ZeroDriftGenerator() :
    vol_(0),
    dt_(1),
    sims_(1),
    price_(10, 0.0, vol_, 1)
  {
  }

  void reset(double vol, double dt, int sims, unsigned int seed)
  {
    vol_ = vol;
    dt_ = dt;
    if (dt < 0) dt *= 1;
    sims_ = sims;
    if (seed == 0) seed = 1;
    seed_ = seed;
    price_ = Diffuse::PriceSimulation(0.0, 0.0, vol_, dt_, seed);
  }


  double Seed() const { return seed_; }
  double Vol() const { return vol_; }
  double Dt() const { return dt_; }
  int Sims() const { return sims_; }

  std::vector<double> prices() const 
  {
    std::vector<double> prices;

    for(int i=0; i<sims_; ++i)
    {
      prices.push_back(price_.Next());
    }
    return prices;
  }

private:
  double vol_;
  double dt_;
  int sims_;
  double seed_;
  mutable Diffuse::PriceSimulation price_;
};

std::ostream & operator << (std::ostream & os, const ZeroDriftGenerator & gen)
{
  os << "Seed " << gen.Seed() << ", drift " << gen.Vol() << ", dt " << gen.Dt() << ", sims " << gen.Sims() << '\n';
    for(const auto & price : gen.prices())
  {
    os << price << '\n';
  }
  return os;
}

void generate(size_t n, ZeroDriftGenerator & out)
{
  double vol;
  double dt;
  int sims;
  unsigned int seed;
  generate(n, vol);
  generate(n, dt);
  generate(n, sims);
  generate(n, seed);
  if (dt < 0) dt *= -1;//needs to be positive
  out.reset(vol, dt, sims, seed);
}

class ZeroDriftPricesNeverNegative : public Property<ZeroDriftGenerator> {

  bool holdsFor(const ZeroDriftGenerator& gen)
  {
    std::vector<double> xs = gen.prices();
    for(const auto & p : xs)
    {
      if (p < 0.0) return false;//TODO calculate the avergave
    }
    return true;
  }

  bool accepts(const ZeroDriftGenerator& gen)
  {
    return gen.Dt() > 0.0
      && gen.Seed() > 0;//for VS2012
  }
};



int main()
{
  //check<ZeroVolGivesDrift>("testing", 10, true);//verbose output *before* testing
  ZeroVolGivesDrift zeroVol;
  zeroVol.check(250);

  ZeroStartPriceGivesZero zeroStartPrice;
  zeroStartPrice.check(100);

  ZeroDriftPricesNeverNegative zeroDriftPricesNeverNegative;
  zeroDriftPricesNeverNegative.check(2500);

  //Could add mean jumps test
}
