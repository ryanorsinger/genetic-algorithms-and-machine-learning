/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
describe("best", function() {

  it("should return the highest particle", function() {
    low  = { x: 5, y: 0 };
    high = { x: 5, y: 8 };
    expect(best(low, high).y).toEqual(high.y);
  });

  it("happens to return the second of two equally high particles", function() {
    first  = { x: 1, y: 8 };
    second = { x: 2, y: 8 };
    expect(best(first, second).x).toEqual(second.x);
  });
});

describe("getRandomInt", function() {
  it("should return a number in [min, max]", function() {
    var value = getRandomInt(0, 11);
    expect(value).not.toBeLessThan(0);
    expect(value).not.toBeGreaterThan(11);
  });
});

describe("updateBest", function() {
  it("should update the global best", function() {
    var particles = [
      { x: 0, y: 0, best: {x:0, y:0} },
      { x: 0, y: 5, best: {x:0, y:0} }
    ];
    var bestGlobal = {x:0, y:0};
    bestGlobal = updateBest(particles, bestGlobal);
    expect(bestGlobal.y).toEqual(5);
  });

  it("should update the personal best", function() {
    var particles = [
      { x: 0, y: 1, best: {x:0, y:0} },
      { x: 0, y: 5, best: {x:0, y:0} }
    ];
    var bestGlobal = {x:0, y:0};
    bestGlobal = updateBest(particles, bestGlobal);
    expect(particles[0].best.y).toEqual(1);
    expect(particles[1].best.y).toEqual(5);
  });
});

describe("makeParticles", function() {
  it("should return the requested number of particles", function() {
    var width = 250;
    var height = 250;
    expect(makeParticles(1, width, height).length).toEqual(1);
    expect(makeParticles(2, width, height).length).toEqual(2);
    expect(makeParticles(3, width, height).length).toEqual(3);
  });

  it("should set best to initial position", function() {
    var width = 250;
    var height = 250;
    var item = makeParticles(3, width, height);
    var i;
    for(i=0; i<item.length; ++i) {
      var current = item[i];
      expect(item[i].best.x).toEqual(item[i].x);
      expect(item[i].best.y).toEqual(item[i].y);
      expect(current.best.x).toEqual(current.x);
      expect(current.best.y).toEqual(current.y);
    }
  });
});

describe("move", function() {
  it("should keep x inside width of bag for each step", function() {
    var width = 250;
    item = { x: 0, y: 0, best: {x:0, y:0}, velocity: { x: 1, y: 1} };
    move_in_range(-10, width, item, "x");
    expect(item.x).toEqual(0);
  });
  it("should keep y inside width of bag for each step", function() {
    var height = 20;
    item = { x: 0, y: 0, best: {x:0, y:0}, velocity: { x: 1, y: 1} };
    move_in_range(100, height, item, "y");
    expect(item.y).toEqual(height);
  });
  it("should move y if possible", function() {
    var height = 20;
    item = { x: 0, y: 0, best: {x:0, y:0}, velocity: { x: 1, y: 1} };
    move_in_range(1, height, item, "y");
    expect(item.y).toEqual(1);
  });
  it("should move x if possible", function() {
    var width = 20;
    item = { x: 0, y: 0, best: {x:0, y:0}, velocity: { x: 1, y: 1} };
    move_in_range(1, width, item, "x");
    expect(item.x).toEqual(1);
  });
});

describe("makeParticles", function() {
  it("should return the requesed number of particles", function() {
    var width = 250;
    var height = 250;
    expect(makeParticles(1, width, height).length).toEqual(1);
    expect(makeParticles(2, width, height).length).toEqual(2);
    expect(makeParticles(3, width, height).length).toEqual(3);
  });

  it("should set best to initial position", function() {
    var width = 250;
    var height = 250;
    var item = makeParticles(3, width, height);
    var i;
    for(i=0; i<item.length; ++i) {
      var current = item[i];
      expect(item[i].best.x).toEqual(item[i].x);
      expect(item[i].best.y).toEqual(item[i].y);
      expect(current.best.x).toEqual(current.x);
      expect(current.best.y).toEqual(current.y);
    }
  });

  it("should set a velocity", function(){
    var width = 250;
    var height = 250;
    var item = makeParticles(1, width, height);
    var velocity = item[0].velocity;
    expect(velocity.x).not.toBeGreaterThan(5);
    expect(velocity.x).not.toBeLessThan(-5);
    expect(velocity.y).not.toBeGreaterThan(5);
    expect(velocity.y).not.toBeLessThan(0);
  });
});


