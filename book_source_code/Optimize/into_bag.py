import argparse
import inspect
import math

from demo import Demo
import hill_climb
import sim_anneal
import turtle

def stuck():
  turtle.setworldcoordinates(-12, -1, 12, 15)
  f = lambda x: math.fabs(x)
  demo = Demo(f)
  start = -10
  step = 3 
  demo.start(start)
  demo.bag(range(-10, 11))
  gen = hill_climb.seek(start, step, f)
  for x, y in gen:
    demo.move(x, y, False)



def quadratic():
  turtle.setworldcoordinates(-12, -2, 12, 102)
  f = lambda x: x**2
  demo = Demo(f)
  demo.start(-10)
  demo.bag([x*0.5 for x in range(-20, 21)])
  gen = hill_climb.seek(-10, 0.5, f)
  for x, y in gen:
    demo.move(x, y, False)
  

def cosine():
  turtle.setworldcoordinates(-6.2, -12, 6.2, 12)
  f = lambda x: 10*math.cos(x)
  demo = Demo(f)
  demo.start(-6)
  demo.bag([x*0.1 for x in range(-62, 62)])
  gen = hill_climb.seek(-6, 0.1, f)
  for x, y in gen:
    demo.move(x, y, False)

def cosine_slope():
  turtle.setworldcoordinates(-6.2, -12, 6.2, 12)
  f = lambda x: -x+5*math.cos(x)
  demo = Demo(f)
  demo.start(-6)
  demo.bag([x*0.1 for x in range(-62, 62)])
  gen = hill_climb.seek(-6, 0.1, f)
  for x, y in gen:
    demo.move(x, y, False)

def slanty_bag_curve(x):
  left = 0.5
  width = 9.
  if x < left:
    y = -20.*x+10.
  elif x < width + left:
    y = 0
  else:
    y = 20.*x-190
  return y

def slanty_bag():
  turtle.setworldcoordinates(-2.2, -2, 12.2, 22)
  demo = Demo(slanty_bag_curve)
  demo.bag([x*0.5 for x in range(-1, 22)])

  x = -0.5
  step = 0.1
  demo.start(x)
  gen = hill_climb.seek(x, step, slanty_bag_curve)
  for x, y in gen:
    demo.move(x, y, False)


def bounds(bounded, x_points):
  if bounded:
    return x_points[0], x_points[-1]
  return float('-inf'), float('inf')

def sa_demo(curr_x,
      step,
      f,
      temperature,
      x_points,
      min_x, max_x,
      *setup):
  turtle.setworldcoordinates(*setup)
  demo = Demo(f)
  demo.start(curr_x)
  demo.bag(x_points)
  gen = sim_anneal.seek(curr_x, step, f, temperature, min_x, max_x)
  for x, y, t, j in gen:
    demo.move(x, y, j)
    curr_x = x
  print(curr_x, f(curr_x))
  

def sa_cosine_turtles(bounded):
  turtle.setworldcoordinates(-6.2, -12, 6.2, 12)
  curr_x = [-6.0, 0, +6.0]
  f = lambda x: 10*math.cos(x)
  count = 3
  demo = [Demo(f) for _ in range(count)]
  x_points = [x*0.1 for x in range(-62, 62)]
  demo[0].bag(x_points)
  min_x, max_x = bounds(bounded, x_points)
  gens = []
  temperature = 10.0
  step = 0.2
  for i, x in enumerate(curr_x):
    demo[i].start(curr_x[i])
    gens.append(sim_anneal.seek(x, step, f, temperature, min_x, max_x))
  for (x1, y1, t1, j1), (x2, y2, t2, j2), (x3, y3, t3, j3) in zip(*gens):
      demo[0].move(x1, y1, j1)
      demo[1].move(x2, y2, j2)
      demo[2].move(x3, y3, j3)

def sa_quad(bounded):
  temperature = 10.0
  step = 0.2
  curr_x = -10.0
  f = lambda x: x**2
  x_points = [x*0.5 for x in range(-20, 21)]
  min_x, max_x = bounds(bounded, x_points)
  sa_demo(curr_x, step, f, temperature, x_points, min_x, max_x, -12, -2, 12, 102)

def sa_cosine_slope(bounded):
  f = lambda x: -x+5*math.cos(x)
  x_points = [x*0.1 for x in range(-62, 62)]
  min_x, max_x = bounds(bounded, x_points)
  temperature = 12
  step = 0.2 
  sa_demo(x_points[0], step, f, temperature, 
           x_points,
           min_x, max_x,
           -6.2, -12, 6.2, 12)

def sa_slanty_bag(bounded):
  x_points = [x*0.5 for x in range(-1, 22)]
  min_x, max_x = bounds(bounded, x_points)
  temperature = 5.0
  step = 0.1
  sa_demo(x_points[0], step, slanty_bag_curve, temperature, 
           x_points,
           min_x, max_x,
           -2.2, -2, 12.2, 22)


def sa_stuck(bounded): 
  f = lambda x: math.fabs(x)
  x_points = [x for x in range(-10, 11)]
  min_x, max_x = bounds(bounded, x_points)
  temperature = 12
  step = 0.1
  sa_demo(x_points[0], step, f, temperature, 
           x_points,
           min_x, max_x,
           -12, -1, 12, 15)

if __name__ == '__main__':
  def everything(bounded):
    for f in fns.values():
      turtle.clearscreen()
      turtle.title(str(f))
      if f == everything:
        continue
      elif len(inspect.getargspec(f).args)==1:
        print("calling ", f, " with [", bounded, "]" )
        f(bounded)
      else:
        print(inspect.getargspec(f).args)
        print("calling ", f, "()") 
        f()

  fns = {'slanty_bag' : slanty_bag,
    'sa_slanty_bag' : sa_slanty_bag,
    'quadratic' : quadratic,
    'cosine' : cosine,
    'cosine_slope' : cosine_slope,
    'sa_quad' : sa_quad,
    'sa_cosine_slope' : sa_cosine_slope,
    'sa_cosine_turtles' : sa_cosine_turtles,
    "stuck" : stuck,
    "sa_stuck" : sa_stuck,
    "all" : everything }

  parser = argparse.ArgumentParser()
  parser.add_argument("-f", "--function", 
      choices = fns,
      help="One of " + ', '.join(fns.keys()))
  parser.add_argument("--bounded", action="store_true", help="Stops turtle leaving edges")
  args = parser.parse_args()
  try:
    f = fns[args.function]
    turtle.title(args.function)
    if len(inspect.getargspec(f).args)==1:
      f(args.bounded)
    else:
      f()
    turtle.mainloop()
  except KeyError:
    parser.print_help()


#https://stackoverflow.com/questions/37619994/how-do-you-make-python-turtle-stop-moving
#onkey then listen
#but need some way to spot this
