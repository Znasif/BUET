public class TestPoint {

 	public static void main(String args[]) {
 		Point p1 = new Point(3, 4);
	   	p1.print();
   		System.out.println("x = "  + p1.getX());
   		System.out.println("y = "  + p1.getY());
   		p1.setX(6);
   		p1.setY(8);
   		p1.print();
   
   		Point p2 = new Point();
	   	p2.print();
 	}
}