/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
describe("knn", function() {
  it("should give one particle as nearest if there is only one other particle", function() {
     var neighbour = knn([new Particle(0, 0, 0, 0), new Particle(0, 0, 0, 0)], 0, 1);
     expect(neighbour.length).toEqual(1);
     expect(neighbour[0].index).toEqual(1);
  });
});


describe("euclidean_distance", function() {
  it("should give zero from point to itself", function() {
    var particle = new Particle(0, 0, 0, 0);
    expect(euclidean_distance(particle, particle)).toEqual(0);
  });

  it("should give 5 for a 3, 4, 5 triangle", function() {
    var particle1 = new Particle(0, 0, 0, 0);
    var particle2 = new Particle(3, 4, 0, 0);
    expect(euclidean_distance(particle1, particle2)).toEqual(5.0);
  });
});


describe("x_nudge", function() {
  it("should give 0 for no neighbours", function() {
    expect(nudge([], [], "x")).toEqual(0);
  });
  
  it("should step all the way towards one neighbour", function() {
    var pos = {x: 1, y: 2}
    expect(nudge([{index:0}], [pos], "x")).toEqual(pos.x);
  });
});

describe("y_nudge", function() {
  it("should give 0 for no neighbours", function() {
    expect(nudge([], [], "y")).toEqual(0);
  });
  
  it("should step all the way towards one neighbour", function() {
    var pos = {x: 1, y: 2}
    expect(nudge([{index:0}], [pos], "y")).toEqual(pos.y);
  });
});
