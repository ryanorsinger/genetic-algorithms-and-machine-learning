#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include <catch.hpp> //https://github.com/philsquared/Catch.git

//first parametrer is test name
//second is a tag for filtering, like
//UnitTests.exe [Bees]

#include "../Bees/Bee.h"

#include <random>


TEST_CASE("Test that bee has initially stated job", "[Bees]")
{
	using namespace BeeColony;
	Role role = Role::Worker;
	Bee bee(role);
	REQUIRE(bee.get_role() == role);
}

TEST_CASE("Test that update_bees keeps same number of Worker bees", "[Bees][Update]")
{
	using namespace BeeColony;
	Hive hive(3, 2, 1, { 0, 0 }, { 0, 0 }, 10.0f, 100);
	hive.update_bees();
	auto updated = hive.get_bees();
	int worker = 0;
	for (const auto & bee : updated) //count_if
	{
		if (bee.get_role() == Role::Worker)
			++worker;
	}
	REQUIRE(worker == 3);
}

TEST_CASE("Test that update_bees keeps same number of Inactive bees", "[Bees][Update]")
{
	using namespace BeeColony;
	Hive hive(3, 2, 1, { 0, 0 }, { 0, 0 }, 10.0f, 100);
	hive.update_bees();
	auto updated = hive.get_bees();
	int inactive = 0;
	for (const auto & bee : updated) //count_if
	{
		if (bee.get_role() == Role::Inactive)
			++inactive;
	}
	REQUIRE(inactive == 2);
}

TEST_CASE("Test that update_bees keeps same number of Scout bees", "[Bees][Update]")
{
	using namespace BeeColony;
	Hive hive(3, 2, 1, { 0, 0 }, { 0, 0 }, 10.0f, 100);
	hive.update_bees();
	auto updated = hive.get_bees();
	int scout = 0;
	for (const auto & bee : updated) //count_if
	{
		if (bee.get_role() == Role::Scout)
			++scout;
	}
	REQUIRE(scout == 1);
}

TEST_CASE("Test that Inactive bee reports it is home", "[Bees][Home]")
{
	using namespace BeeColony;
	Bee bee(Role::Inactive);
	REQUIRE(bee.is_home());
}

TEST_CASE("Test that hive of Inactive bee reports they are home", "[Bees][Home]")
{
  using namespace BeeColony;
  Hive hive(0, 1, 0, { 0, 0 }, { 0, 0 }, 10.0f, 100);
  REQUIRE(hive.all_home());
}

TEST_CASE("Test that make_hive starts with bees at home", "[Bees][Home]")
{
  const int number_workers = 1;
  const int number_inactive = 1;
  const int number_scout = 1;
  const int steps = 100;
  const float width = 500.0f;
  const float bee_size = 10.0f;
  const BeeColony::Coordinate food{ 100.0f, 0.0f };
  BeeColony::Hive hive(number_workers, number_inactive, number_scout,
    {width/2, 10.0f}, food, bee_size, steps);
  auto bees = hive.get_bees();
  for(auto  bee : bees)
	  REQUIRE(bee.is_home());
}

TEST_CASE("Test that a Scout bee becoming a Worker bee remembers its best food source", "[Bees][JobUpdate]")
{
	using namespace BeeColony;
	double start_food_x = 100.0;
	double start_food_y = 300.0;
	Bee bee(Role::Scout, { 0.0, 0.0 }, { start_food_x, start_food_y }, 5.0);
	double initialQuality = quality(bee.get_pos());
	bee.communicate(Role::Worker, { 200, 0 });
	REQUIRE(BeeColony::quality(bee.get_food()) > initialQuality);
}

TEST_CASE("Test that a Worker bee becoming a Scout bee takes on board better food source", "[Bees][JobUpdate]")
{
	using namespace BeeColony;
	double start_food_x = 100.0;
	double start_food_y =   0.0;
	Bee bee(Role::Worker, { 0.0, 0.0 }, { start_food_x, start_food_y }, 5.0);
	double initialQuality = quality(bee.get_pos());
	bee.communicate(Role::Scout, { 200, 400 });
	REQUIRE(BeeColony::quality(bee.get_food()) > initialQuality);
}

TEST_CASE("Test that Scout will try anything when on first outing", "[Bees][Scout]")
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(Role::Scout, { 0.0, 0.0 }, { 0.0, 0.0 }, buzz);
	const double move = 10;
	bee.scout(0, move);
	REQUIRE(!bee.is_home());
	REQUIRE(quality(bee.get_pos()) == move * buzz);
	while(!bee.is_home())
		bee.go_home();
	REQUIRE(bee.is_home());
	bee.communicate(Role::Scout, { 0, 0 });
	bee.scout(0, move/2);
	REQUIRE(!bee.is_home());
	REQUIRE(quality(bee.get_pos()) == move/2 * buzz);
}

TEST_CASE("Test that Worker bees will go home", "[Bees][Home]")
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(Role::Worker, { 0.0, 0.0 }, { 0.0, 0.0 }, buzz);
	bee.work(10, 5);
	REQUIRE(!bee.is_home());
	while(!bee.is_home())
	{
		double last_x = bee.get_pos().x;
		bee.go_home();
		REQUIRE(bee.get_pos().x < last_x);
	}
}

TEST_CASE("Test that waggle_dance bees swaps jobs", "[Bees][waggle_dance]")
{
	using namespace BeeColony;
	const float buzz = 5.0f;
	Hive hive(1, 1, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, buzz, 1);
	hive.update_bees();//calls waggle_dance
	auto bees = hive.get_bees();
	REQUIRE(bees.size() == 2);
	auto first = bees[0].get_role();
	auto second = bees[1].get_role();
	REQUIRE(first != Role::Scout);
	REQUIRE(second != Role::Scout);
	if (first == Role::Inactive)
		REQUIRE(second == Role::Worker);
	else
		REQUIRE(second == Role::Inactive);
}

TEST_CASE("Test that waggle_dance bees keeps best food source", "[Bees][waggle_dance]")
{
	using namespace BeeColony;
	const float buzz = 5.0f;
	Hive hive(1, 1, 0, { 0.0, 0.0 }, { 0.0, 0.0 }, buzz, 1);
	hive.update_bees();//calls waggle_dance
	auto bees = hive.get_bees();
	REQUIRE(bees.size() == 2);
	auto first = bees[0].get_food();
	auto second = bees[1].get_food();
	REQUIRE(quality(first) == quality(second));
}

TEST_CASE("Test that rubbish bees do not swarm", "[Bees][Swarm]")
{
	using namespace BeeColony;
	std::vector<Bee> bees{ Bee(Role::Worker, {0.0, 0.0}, {0.0, 0.0}, 0.0) };
	REQUIRE(!should_swarm(bees, 10));
}

TEST_CASE("Test that good bees do swarm", "[Bees][Swarm]")
{
	using namespace BeeColony;
	std::vector<Bee> bees{ Bee(Role::Worker, {0.0, 0.0}, {15.0, 15.0}, 0.0) };
	REQUIRE(should_swarm(bees, 10));
}

void testHome(BeeColony::Role job)
{
	using namespace BeeColony;
	const double buzz = 5.0;
	Bee bee(Role::Scout, { 5.0, 5.0 }, { 8.0, 8.0 }, buzz);
	bee.move_home({ 10.0, 10.0 });
	REQUIRE(bee.get_food().x == 8.0);
	REQUIRE(bee.get_food().y == 8.0);
	bee.go_home();
	bee.go_home();
	REQUIRE(bee.get_pos().x == 10.0);
	REQUIRE(bee.get_pos().y == 10.0);
}

TEST_CASE("Test that bee below a new home goes towards new home when told", "[Bees][goHome]")
{
	testHome(BeeColony::Role::Worker);
	testHome(BeeColony::Role::Inactive);
	testHome(BeeColony::Role::Scout);
}

TEST_CASE("Test that move gets nearer destination", "[Bees][move]")
{
	using namespace BeeColony;
	Coordinate from{ 0.0f, 0.0f };
	const Coordinate to{ 2.0f, 2.0f };
	const double step = 0.5;

	move(from, to, step);
	REQUIRE(from.x > 0.0f);
	REQUIRE(from.y > 0.0f);
}