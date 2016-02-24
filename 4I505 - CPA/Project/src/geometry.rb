# Module used for all geometry tasks.
module Geometry
  # Represent a point.
  class Point
    # Access for coordinates. A point is not mutable.
    attr_reader :x, :y

    def initialize(x, y)
      @x = x
      @y = y
    end

    # Return distance between self and point.
    def distance(point)
      Math.sqrt((point.x - self.x)**2 + (point.y - self.y)**2)
    end

    # Return the middle point between self and point.
    def middle(point)
      Point.new((point.x + self.x) / 2, (point.y + self.y) / 2)
    end

    # Override equality, just in case.
    def ==(point)
      point.x == self.x and point.y == self.y
    end
  end

  # Represent a circle.
  class Circle
    # Access for center and radius.
    attr_reader :center, :radius

    def initialize(center, radius)
      @center = center
      @radius = radius
    end

    # Indicates if point is covered by circle.
    def is_covered(point)
      center.distance(point) <= radius
    end

    # Equality, just in case.
    def ==(circle)
      circle.center == self.center and circle.radius == self.radius
    end

    def contains(point)
      center.distance(point) < radius
    end
  end

  ##################
  # MODULE METHODS #
  ##################

  # Return the distance between two points.
  def self.distance(point1, point2)
    point1.distance(point2)
  end

  # Return the middle point between two points.
  def self.middle(point1, point2)
    point1.middle(point2)
  end

  # Return the circumcircle of the three points.
  def self.circumcircle(point1, point2, point3)
    x1 = point1.x.to_f
    x2 = point2.x.to_f
    x3 = point3.x.to_f

    y1 = point1.y.to_f
    y2 = point2.y.to_f
    y3 = point3.y.to_f

    return nil if (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) == 0

    x = (((x3**2 - x2**2 + y3**2 - y2**2) /
    (2 * (y3 - y2))) -
    ((x2**2 - x1**2 + y2**2 - y1**2) /
    (2 * (y2 - y1)))) /
    (((x2 - x1) / (y2 - y1)) -
    ((x3 - x2) / (y3 - y2)))

    y = (-((x2 - x1) / (y2 - y1))) * x +
    ((x2**2 - x1**2 + y2**2 - y1**2) / (2 * (y2 - y1)))

    r = Math.sqrt((x1 - x)**2 + (y1 - y)**2)

    Circle.new(Point.new(x, y), r)
  end
end
