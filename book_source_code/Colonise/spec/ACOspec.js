/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
describe("start_pos", function() {
  it("should not go beyond the edge", function() {
    var pos = start_pos(1);
    expect(pos.x).toBeLessThan(2);
  });

  it("should start at the bottom of the bag", function() {
    var pos = start_pos(1);
    expect(pos.y).toBe(0);
  });
});

describe("next_pos", function() {

  //using (x,y) mathematically - so 0 is at the bottom
  //the drawing code flips them - as 0 is at the top on the canvas

  it("should not be below bag", function() {
    var width = 4;
    for(var i=0; i<width; ++i)
    {
      var pos = { x:i, y:0 };
      var next = next_pos(width, pos, []);
      expect(next.y).not.toBeLessThan(0);
    }
  });


  it("should not be beside bag", function() {
    var width = 4;
    for(var i=0; i<width; ++i)
    {
      var pos = { x:i, y:0 };
      var next = next_pos(width, pos, []);
      expect(next.x >= 0).toBe(true);
      expect(next.x).not.toBeGreaterThan(width);
    }
  });


  it("should not return to a current point", function() {
    expect(true).toBe(true);
    var width = 4;
    var pos = { x:0, y:1 };
    var path = [];
    var old_pos = { x:0, y:1 };
    path.push( old_pos );
    var new_pos = next_pos(width, pos, path);
    expect(new_pos.x === 0 && new_pos.y === 1).toBe(false);
  });

  it("should not return to a previous point", function() {
    expect(true).toBe(true);
    var width = 4;
    var pos = { x:0, y:0 };
    var path = [];
    path.push( {x:0, y:0 } );
    path.push( {x:0, y:1 } );
    path.push( {x:1, y:0 } );

    var new_pos = next_pos(width, pos, path);
    expect(new_pos.x).toEqual(1);
    expect(new_pos.y).toEqual(1);
  });


});

describe("allowed_positions", function() {

  it("should not go to a position that has already been visited", function() {
    var pheromones = [], path = [], pos = {x: 0, y: 0};
 
    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 0, weight: 0});

    path.push( { x: 0, y: 0 } );
    path.push( { x: 0, y: 1 } );

    var allowed = allowed_positions(4, pos, path);
    expect(allowed.length).toEqual(2);
  });
});

describe("random_path", function() {

  it("should start at bottom of bag", function() {
    var width = 4;
    var height = 3;
    var path = random_path(height, width);
    expect(path.length>0).toBe(true);
    expect(path[0].y).toEqual(0);
  });

  it("should end above bag", function() {
    var width = 4;
    var height = 3;
    var path = random_path(height, width);
    expect(path.length).toBeGreaterThan(0);
    expect(path[path.length-1].y).not.toBeLessThan(height);
  });

});

describe("make_paths", function() {

  it("should return same number of paths as ants", function() {
    var height = 5;
    var width = 5;
    var ants = 10;
    var paths = make_paths(height, width, ants);
    expect(paths.length).toBe(ants);
  });

});

describe("total_length", function() {
  it("will see a straight line as shorter than a diagonal line", function() {
    path_shorter = [];
    path_shorter.push({x: 0, y: 1});
    path_shorter.push({x: 0, y: 0});

    path_longer = [];
    path_longer.push({x: 1, y: 1});
    path_longer.push({x: 0, y: 0});

    var short = total_length(path_shorter);
    var long = total_length(path_longer);
    expect(short).toBeLessThan(long);
  });

  it("will see a straight line of three steps as length three", function() {
    path = [];
    path.push({x: 0, y: 3});
    path.push({x: 0, y: 0});

    var length = total_length(path);
    expect(length).toEqual(3);
  });

  it("will be greater than zero for a path of several steps", function() {
    var paths = [];
    paths.push( [{x:120, y:260},  {x:150, y:230},  {x:180, y:200},  {x:180, y:170},  {x:150, y:140},  {x:150, y:110},  {x:150, y:80},  {x:180, y:50},  {x:180, y:20},  {x:150, y:-10}] );
    expect(total_length(paths[0])).toBeGreaterThan(0);
  });
});

describe("find_best", function() {

  it("should find the only path if there is just one", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_best(paths)).toBe(0);
  });

  it("should find the shortest path when it is first", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    expect(find_best(paths)).toBe(0);
  });

  it("should find the shortest path when it is last", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_best(paths)).toBe(1);
  });
});

describe("find_worst", function() {

  it("should find the only path if there is just one", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_worst(paths)).toBe(0);
  });

  it("should find the longest path when it is first", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_worst(paths)).toBe(0);
  });

  it("should find the longest path when it is last", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}, {x:4, y:0}, {x:5, y:0}] );
    expect(find_worst(paths)).toBe(1);
  });
});

describe("find_average", function() {

  it("should return length of path if there is just one just one", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_average(paths)).toEqual(total_length(paths[0]));
  });

  it("should return length of path if there all the same length", function() {
    var paths = [];
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_average(paths)).toEqual(total_length(paths[0]));
    paths.push( [{x:2, y:0}, {x:3, y:0}] );
    expect(find_average(paths)).toEqual(total_length(paths[0]));
  });

  it("should match best and worst for a single path", function() {
    var paths = [];
    paths.push( [{x:120, y:260},  {x:150, y:230},  {x:180, y:200},  {x:180, y:170},  {x:150, y:140},  {x:150, y:110},  {x:150, y:80},  {x:180, y:50},  {x:180, y:20},  {x:150, y:-10}] );
    var average = find_average(paths);
    expect(average).toEqual(total_length(paths[find_best(paths)]));
    expect(average).toEqual(total_length(paths[find_worst(paths)]));
  });
});

describe("contains", function() {

  it("should not contain a item when it is empty", function() {
    var paths = []
    var pos = { x: 0, y: 1};
    expect(contains(paths, pos)).toBe(false);
  });

  it("should contain a item when it is the only item", function() {
    var paths = []
    var pos = { x: 0, y: 1};
    paths.push( {x: 0, y:1 } ); 
    expect(contains(paths, pos)).toBe(true);
  });

});

describe("add_new_pheromones", function () {

  it("should contain each point in a new path", function() {
    var i, pos, pheromones = [], updated;
    var width = 4;
    var height = 6;
    var path = random_path(height, width);
    add_new_pheromones(height, pheromones, path);
    //there may be some repeated points in the path
    expect(pheromones.length).not.toBeGreaterThan(path.length);

    for( i = 0; i < path.length; ++i) {
      pos = path[i];
      for(j = 0; j<pheromones.length; ++j) {
        expect(contains(pheromones, pos)).toBe(true);
      }
    }
  });
});

describe("tau eta", function () {
  it("should be larger for each higher up point", function() {
    var low = taueta(10, 1);
    var high = taueta(10, 5);
    expect(high).not.toBeLessThan(low);
  });

  it("should make places with more pheromone more appealing", function() {
    var low = taueta(10, 1);
    var high = taueta(20, 1);
    expect(high).not.toBeLessThan(low);
  });

  it("can be zero", function() {
    expect(taueta(10,  0)).toBe(0);
    expect(taueta( 0, 10)).toBe(0);
  });
});

describe("evaporate", function() {
  it("should decrease the pheromones", function() {
    var original_value= 100;
    var pheromones = [{x:0, y:0, weight:original_value}];
    evaporate(pheromones);
    expect(pheromones[0].weight).toBeLessThan(original_value);
  });
});

describe("update", function() {
  it("should add pheromone where ant went", function() {
    var height = 5;
    var width = 5;
    var ants = 1;
    var path = make_paths(height, width, ants);
    var pheromones = [];
    update(pheromones, path, height);
    for (i = 0; i < path.length; ++i) {
      pos = path[0][i];
      index = pheromone_at(pheromones, pos);
      expect(index).not.toBe(-1);
      if ( index !== -1 ) {
        expect(pheromones[index].weight).toBeGreaterThan(0);
      }
    }
  });

  it("should not add pheromone where ant did not go", function() {
    var height = 5;
    var width = 5;
    var ants = 1;
    var path = make_paths(height, width, ants);
    var pheromones = [];
    update(pheromones, path, height);
    for (i = 0; i < pheromones.length; ++i) {
      pos = pheromones[i];
      expect(contains(path[0], pos)).toBe(true);
    }
  });
});

describe("find pheromone", function() {
  it("should return -1 when none at that point", function () {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    var index = pheromone_at(pheromones, pos);
    expect(index === -1).toBe(true);
  });

  it("should return point's index when point is there", function () {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    pheromones.push(pos);
    var index = pheromone_at(pheromones, pos);
    expect(index === 0).toBe(true);
  });

});

describe("possible_positions", function() {
  it("should not go below bottom of bag", function() {
    var width = 10;
    var possible = possible_positions(width, {x:0, y:0});
    expect(possible).not.toEqual(jasmine.objectContaining({y:-1}));
  });

  it("should not go left of bag", function() {
    var width = 10;
    var possible = possible_positions(width, {x:0, y:0});
    expect(possible).not.toEqual(jasmine.objectContaining({x:-1}));
  });

  it("should not go right of bag", function() {
    var width = 10;
    var possible = possible_positions(width, {x:0, y:0});
    expect(possible).not.toEqual(jasmine.objectContaining({x:width+1}));
  });
});

describe("cumulative", function() {
  it("should give the sum of tau eta when there is one point", function() {
    var pheromones = [];
    var pos = {x: 1, y: 2, weight: 1};
    pheromones.push(pos);
    var possible = [];
    possible.push( { x: 1, y: 2 } );
    var cumulative = partial_sum(possible, pheromones); 
    expect(cumulative.length === 2).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(taueta(pos.weight, pos.y));
  });

  it("should give the sum of tau eta when there is a non-zero weight and the rest are zero", function() {
    var pheromones = [], possible = [];

    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 1, weight: 0});
    pheromones.push({x: 1, y: 0, weight: 0});

    possible.push( { x: 0, y: 1 } );
    possible.push( { x: 1, y: 1 } );
    possible.push( { x: 1, y: 0 } );

    var cumulative = partial_sum(possible, pheromones);
    expect(cumulative.length === 4).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(taueta(1,1));
    expect(cumulative[2]).toEqual(taueta(1,1) + taueta(0,1));
    expect(cumulative[3]).toEqual(taueta(1,1) + taueta(0,1) + taueta(0,0));
  });

  it("should give zero if no points have pheromones", function() {
    var pheromones = [], possible = [];

    pheromones.push({x: 5, y: 5, weight: 1});

    possible.push( { x: 0, y: 1 } );
    possible.push( { x: 1, y: 1 } );
    possible.push( { x: 1, y: 0 } );

    var cumulative = partial_sum(possible, pheromones);
    expect(cumulative.length === 4).toBe(true);
    expect(cumulative[0]).toEqual(0);
    expect(cumulative[1]).toEqual(0);
    expect(cumulative[2]).toEqual(0);
    expect(cumulative[3]).toEqual(0);
  }); 

});

describe("taueta", function() {
  it("should prefer higher up places when weight is the same", function() {
    expect( taueta(1.0, 1) ).toBeLessThan( taueta(1.0, 10) );
  });

  it("should prefer higher weights when height is the same", function() {
    expect( taueta(1.0, 1) ).toBeLessThan( taueta(10.0, 1) );
  });
});


describe("roulette_wheel_choice", function() {

  it("should return a position", function() {
    var width = 5, height = 6, i, pos = {x: 0, y: 0}, pheromones = [];
    var path = random_path(height, width);
    add_new_pheromones(height, pheromones, path);
    var moves = allowed_positions(4, pos, path);
    var new_pos = roulette_wheel_choice(moves, pheromones);
    expect(new_pos.x).not.toBeLessThan(0);
  });

  it("should go to best pheromone point if all other points have zero probability", function() {
    var pheromones = [], possible = [], path = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 1, weight: 1});//best weight and height
    pheromones.push({x: 1, y: 0, weight: 0});

    path.push( { x: 0, y: 0 } );

    var moves = allowed_positions(4, pos, path);
    var new_pos = roulette_wheel_choice(moves, pheromones);
    expect(new_pos.x).toEqual(0);
    expect(new_pos.y).toEqual(1);
  });

  it("should go to a better pheromone point if some other points have zero probability", function() {
    var pheromones = [], possible = [], path = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 0, weight: 0});
    pheromones.push({x: 1, y: 0, weight: 0});
    pheromones.push({x: 2, y: 0, weight: 0});
    pheromones.push({x: 0, y: 1, weight: 1});//equal best weight and height
    pheromones.push({x: 1, y: 1, weight: 1});//equal best weight and height

    var moves = allowed_positions(4, pos, path);
    var new_pos = roulette_wheel_choice(moves, pheromones);
    expect(new_pos.y).toEqual(1);//first two have 0 probablility
  })

  it("should not go to a place already visited", function() {
    var pheromones = [], path = [], pos = {x: 0, y: 0};

    pheromones.push({x: 0, y: 1, weight: 1});
    pheromones.push({x: 1, y: 0, weight: 0});

    path.push( { x: 0, y: 0 } );
    path.push( { x: 0, y: 1 } );
    path.push( { x: 1, y: 1 } );

    var moves = allowed_positions(4, pos, path);
    var new_pos = roulette_wheel_choice(moves, pheromones);
    expect(new_pos.x).toEqual(1);
    expect(new_pos.y).toEqual(0);
  });
});


describe("path_to_string", function() {

  it("should show points)", function() {
    var path = [
                  {x: 4, y: 0}, 
                  {x: 4, y: 1},
                  {x: 3, y: 2},
                  {x: 3, y: 3},
                  {x: 3, y: 4},
                  {x: 3, y: 5},
                  {x: 3, y: 6}
                ];
    //note height should be canvas.height/scale
    // and it is kinda saying the (unsacled) canvas pos, rather than y coord
    expect(path_to_string(6, path)).toEqual(" (4, 6) (4, 5) (3, 4) (3, 3) (3, 2) (3, 1) (3, 0)");
  });

});


