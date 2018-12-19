import math

def seek(x, step, f):
  height = f(x)
  while True:
    if f(x-step) < height: 
      x -= step
    elif f(x+step) <= height: 
      x += step
    else:
      break
    height = f(x)
    yield x, height

def console_show(x, y):
  print (x, y)

if __name__ == '__main__':
  print (climb(-10, 0.5, lambda x: x**2, 0, console_show))
  print (climb(-6, 0.1, lambda x: 10*math.cos(x), 0, console_show))

