import math

radian = 180/math.pi
radius = 6371e3

class point:
    def __init__(self, lat, lon, x , y):
        self.lat=lat
        self.lon=lon
        self.r = 1
        self.x = x*self.r
        self.y = y*self.r
    def translate_coordinates(self, x, y, angle):
        x = self.r*x
        y = self.r*y
        r=math.sqrt(x*x+y*y)
        if(r):
            ct = x/r
            st = y/r
            x = r * ( (ct * math.cos(angle))+ (st * math.sin(angle)) )
            y = r * ( (st * math.cos(angle))- (ct * math.sin(angle)) )
            p = self.lon/radian
            q = self.lat/radian
            p = (111415.13 * math.cos(p))- (94.55 * math.cos(3.0*p)) + (0.12 * math.cos(5.0*p))
            q = 111132.09 - (566.05 * math.cos(2.0*q))+ (1.20 * math.cos(4.0*q)) - (0.002 * math.cos(6.0*q))
            new_point = point(self.lat+y/q, self.lon+x/p, x, y)
            return new_point
    def print_latlon(self):
        print("Latitude : "+str(self.lat)+" Longitude : "+str(self.lon))

def distance(a, c):
    rad1 = a.lat/radian
    rad2 = c.lat/radian
    rad3 = (c.lat-a.lat)/radian
    rad4 = (c.lon-a.lon)/radian
    p =  math.sin(rad3/2) * math.sin(rad3/2) +math.cos(rad1) * math.cos(rad2) * math.sin(rad4/2) * math.sin(rad4/2)
    q = 2 * math.atan2(math.sqrt(p), math.sqrt(1-p))
    return radius*q


def calculate_origin(a, c):
    p = distance(a,c)
    q = math.sqrt((a.x-c.x)*(a.x-c.x)+(a.y-c.y)*(a.y-c.y))
    a.r = p/q
    origin = a.translate_coordinates(-(a.x),a.y,0)
    return origin

def calculate(origin, x,y):
    new_point = origin.translate_coordinates(float(x),-float(y),0)
    return new_point

if __name__ == "__main__":
    print("First Lat and Lon : ",end= "")
    a,b = input().split()
    print("First X and Y : ",end= "")
    ax,by = input().split()
    axx = point(float(a),float(b),ax,by)


    print("Second Lat and Lon : ",end = "")
    c,d = input().split()
    print("Second X and Y : ",end= "")
    cx,dy = input().split()
    cxx = point(float(c),float(d),cx,dy)

    origin = calculate_origin(axx,cxx)

    print("Coordinates : ",end = "")
    x,y = input().split()
    new_point = origin.translate_coordinates(float(x),float(y),0)
    new_point.print_latlon()
