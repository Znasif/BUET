public class TestCircle {

	public static void main(String args[]) {

        Point p1 = new Point(3, 4);
	   	p1.print();
	   	
	   	Point p2 = new Point();
   		System.out.println("x = "  + p2.getX());
   		System.out.println("y = "  + p2.getY());
   		p2.setX(6);
   		p2.setY(8);
   		p2.print();
   	
   		Circle c1 = new Circle(p1, 4, "red");
		c1.print();
	}
}
