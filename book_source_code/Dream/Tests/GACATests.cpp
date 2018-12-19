#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../GACA/GACA.h"
#include <algorithm>
#include <bitset>
#include <sstream> 
using namespace GACA;

namespace Catch {
    std::string toString(const GACA::Row & cells) {
		std::stringstream ss;
		for(const auto & item : cells)
		{
			ss << std::boolalpha << item << " ";
		}
		return ss.str();
    }
}

std::ostream& operator<<(std::ostream & os, const GACA::Row & cell)
{
	for (auto c : cell)
		os << c << ' ';
	return os;
}

TEST_CASE("Different cells are generated", "[setup]")
{
	RowGenerator gen(1);//or some seed
	Row cell1 = gen.generate();
	Row cell2 = gen.generate();
	REQUIRE(cell1 != cell2); //actually could happen
}

TEST_CASE("Static rule when just one is alive does just that", "[rule]")
{
	Row cells{ true, false };
	StaticRule rule;
	World world(rule, cells, 1);//yet another magic number
	REQUIRE(1 == fitness(world.State(), true));
}


TEST_CASE("Static rule remembers previous state", "[alive]")
{
	Row cells{ true, false };
	StaticRule rule;
	World world(rule, cells, 10);//yet another magic number
	REQUIRE( world.Alive(0, 0));
	REQUIRE(!world.Alive(1, 0));
	REQUIRE( world.Alive(0, 1));
	REQUIRE(!world.Alive(1, 1));
}

TEST_CASE("A cell mated with itself gives the same cells back", "[breed]")
{
	Row cell{ false, false, false };
	Row new_cell = breed(cell, cell, std::tuple_size<Row>::value/2);
	REQUIRE(new_cell == cell);
}

TEST_CASE("DreamRule adds items as required","[update]")
{
	size_t seed = 2;
	DreamRule rule(seed);
	Row cell;
	cell.fill(false);
	Row cell1 = rule(cell);
	Row cell2 = rule(cell);
	REQUIRE(cell1 == cell2);
}

TEST_CASE("Fitness of dead cells is zero for target alive", "[fitness]")
{
    Row cells{ false };
    REQUIRE(fitness(cells, true) == 0);
}

TEST_CASE("Fitness of dead cells is cell count for target dead", "[fitness]")
{
    Row cells{ false };
    REQUIRE(fitness(cells, false) == std::tuple_size<Row>::value);
}

TEST_CASE("Fitness of alive cells is cell count for target alive", "[fitness]")
{
    Row cells;
    cells.fill(true);
    REQUIRE(fitness(cells, true) == std::tuple_size<Row>::value);
}

TEST_CASE("Fitness of alive cells is zero for target dead", "[fitness]")
{
    Row cells;
    cells.fill(true);
    REQUIRE(fitness(cells, false) == 0);
}

TEST_CASE("Tournament selection chooses the best of a given subset", "[fitness]")
{
	std::vector<World> pop;
	StaticRule rule;
	World world1(rule, { false, false, false, false }, 1);
	pop.emplace_back(world1);
	World world2(rule, { true, false, true, false }, 1);
	pop.emplace_back(world2);
	World world3(rule, { false, false, true, false }, 1);
	pop.emplace_back(world3);
	World world4(rule, { true, true, true, true }, 1);
	pop.emplace_back(world4);
	Crossover crossover(1, pop.size(), rule, 1, true, true);
	REQUIRE(fitness(crossover.tournament(pop[0], pop[1], pop[2]).State(), true) == 2);
	REQUIRE(fitness(crossover.tournament(pop[0], pop[2], pop[1]).State(), true) == 2);
	REQUIRE(fitness(crossover.tournament(pop[1], pop[0], pop[2]).State(), true) == 2);
	REQUIRE(fitness(crossover.tournament(pop[1], pop[2], pop[0]).State(), true) == 2);
	REQUIRE(fitness(crossover.tournament(pop[2], pop[0], pop[1]).State(), true) == 2);
	REQUIRE(fitness(crossover.tournament(pop[2], pop[1], pop[0]).State(), true) == 2);
}

TEST_CASE("Tournament selection chooses either of equally good inputs", "[fitness]")
{
	std::vector<World> pop;
	pop.push_back(World(StaticRule(), { false, false }, 1));
	pop.push_back(World(StaticRule(), { false, false }, 1));
	pop.push_back(World(StaticRule(), { true, true }, 1));
	pop.push_back(World(StaticRule(), { true, true }, 1));
	StaticRule rule;
  Crossover crossover(1, pop.size(), rule, 1, true, true);
  REQUIRE(fitness(crossover.tournament(pop[1], pop[2], pop[3]).State(), true) == 2);
  REQUIRE(fitness(crossover.tournament(pop[2], pop[0], pop[3]).State(), true) == 2);
  REQUIRE(fitness(crossover.tournament(pop[0], pop[1], pop[3]).State(), true) == 2);
}

TEST_CASE("Mutation rate of zero changes nothing", "[mutation]")
{
	Mutation mutation(1, 0.0);
	Row cell;
	cell.fill(false);
	Row cell_new = mutation(cell);
	REQUIRE(cell == cell_new);
}

TEST_CASE("Mutation rate of one changes something", "[mutation]")
{
	Mutation mutation(1, 1.0);
	Row cell;
	cell.fill(false);
	Row cell_new = mutation(cell);
	REQUIRE(std::find(cell_new.begin(), cell_new.end(),true) != cell_new.end());
}

TEST_CASE("Crossover keeps the best", "[crossover]")
{
	Row cell_bad;
	cell_bad.fill(false);
	Row cell_better(cell_bad);
	cell_better[1] = true;
	StaticRule rule;
  World world1(rule, cell_bad, 1);
  World world2(rule, cell_better, 1);
  Population pop{ world1, world2 };
  Crossover crossover(0, pop.size(), rule, 1, true, true);
	pop = crossover(pop);
  bool found = (pop[0].State() == cell_better) || (pop[1].State() == cell_better);
	REQUIRE(found);
}

TEST_CASE("Crossover warns on invalid population size", "[crossover]")
{
	StaticRule rule;
	Crossover crossover(0, 100, rule, 1, true, true);
	REQUIRE_THROWS(crossover(Population()));
}

TEST_CASE("Best cell is really the best for no op rule", "[best]")
{
	Row cell_bad;
	cell_bad.fill(false);
	Row cell_better(cell_bad);
	cell_better[1] = true;
  StaticRule rule;
  Population pop{ World(rule, cell_bad, 1), World(rule, cell_better, 1) };
  REQUIRE(fitness(cell_better, true) == 1);
  REQUIRE(fitness(cell_bad, true) == 0);
  REQUIRE(best(pop, true).State() == rule(cell_better));
}


TEST_CASE("Better cell has higher fitness", "[fitness]")
{
	Row cell_off;
	cell_off.fill(false);
	Row cell_one_on(cell_off);
  cell_one_on[1] = true;
	REQUIRE(fitness(cell_one_on, true) > fitness(cell_off, true));
  REQUIRE(fitness(cell_one_on, false) < fitness(cell_off, false));
}

TEST_CASE("Rule0 turns everything off", "[ECA]")
{
    ECARule rule(0);
    Row cell;
    cell.fill(true);
    Row next = rule(cell);
    REQUIRE(fitness(next, true) == 0);
}

TEST_CASE("Rule255 turns everything on", "[ECA]")
{
    ECARule rule(255);
    Row cell;
    cell.fill(false);
    Row next = rule(cell);
    REQUIRE(std::all_of(next.cbegin(), next.cend(), [](bool b) { return b; }));
}

TEST_CASE("Rule2 leaves everything off if it starts off", "[ECA]")
{
    ECARule rule(2);
    Row cell;
    cell.fill(false);
    Row next = rule(cell);
    REQUIRE(fitness(next, true) == 0);
}

TEST_CASE("Rule2 moves a single left cell on left over time", "[ECA]")
{
    ECARule rule(2);
    Row cell;
    cell.fill(false);
    cell.back()=true;
    INFO(Catch::toString(cell));
    REQUIRE( cell[cell.size() - 1]);
    REQUIRE(!cell[cell.size() - 2]);
    Row next = rule(cell);
    INFO(Catch::toString(next));
    REQUIRE(!next[next.size() - 1]);
    REQUIRE( next[next.size() - 2]);
}

TEST_CASE("Update uses rule", "[world]")
{
    Row cell;
    cell.fill(false);
    StaticRule rule;
    World world(rule, cell, 1);
    REQUIRE(world.State() == rule(cell));
}

TEST_CASE("World reset gives new history", "reset")
{
    Row cell;
    cell.fill(true);
    ECARule rule(21);
    World world(rule, cell, 1);
    REQUIRE(fitness(world.State(), true) == 0); //std::tuple_size<Row>::value);
    cell[16] = false;
    world.Reset(cell);
    REQUIRE(fitness(world.State(), true) == 0);
}