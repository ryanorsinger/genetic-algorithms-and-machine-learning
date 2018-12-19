import turtle

class Demo:
  def __init__(self, f):
    self.alex = turtle.Turtle()
    self.alex.shape("turtle") 
    self.f = f

  def bag(self, points):
    line = turtle.Turtle()
    line.pen(pencolor='brown', pensize=5)
    line.up()
    line.goto(points[0], self.f(points[0]))
    line.down()
    for x in points:
      line.goto(x, self.f(x))
    line.hideturtle()

  def start(self, x):
    self.alex.hideturtle()
    self.alex.up()
    self.alex.goto(x, self.f(x))
    self.alex.down()
    self.alex.showturtle()
    self.alex.pen(pencolor='black', pensize=10)
    self.alex.speed(1)

  def move(self, x, y, jump=False):
    if jump: self.alex.up() 
    self.alex.goto(x, y)
    if jump: self.alex.down()
