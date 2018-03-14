public class Circle {

    private double radius;
    private Point center;
    private String color;

    public Circle(Point center, double radius, String color) {
		this.center = center;
		this.radius = radius;
		this.color = color;
	}

    public Point getCenter() {
        return this.center;
    }

    public void print() {
    	System.out.println("Center: (" + getCenter().getX() + "," + getCenter().getY() +
    		") Radius: " + radius + " Color: "  + color);
    }
}