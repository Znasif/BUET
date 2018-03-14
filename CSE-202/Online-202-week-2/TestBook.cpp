#include <iostream>
#include "Book.cpp"
using namespace std;

int main() {
    Author schildt("Herbert Schildt", "herbert@schildt.com");
	cout << schildt.getName() <<  " at " << schildt.getEmail() << endl;

	Author bjarne("Bjarne Stroustrup");
	bjarne.setEmail("bjarne@bjarne.com");
	bjarne.print();

 	Book b1("Teach Yourself C++", schildt);
	b1.setPrice(150.5);
    b1.setStock(50);
    cout << "Author: " << b1.getAuthorName() <<" Name: " << b1.getName() << " Price: " << b1.getPrice() << " Stock: " << b1.getStock() << endl;
    b1.borrowBook(20);
    b1.returnBook(10);
    cout << "Author: " << b1.getAuthorName() <<" Name: " << b1.getName() << " Price: " << b1.getPrice() << " Stock: " << b1.getStock() << endl;


    Book b2("The C++ Programming Language", bjarne, 200, 20);
    b2.print();
    b2.returnBook(10);
    b2.borrowBook(50);
    b2.print();

    b1.getAuthor().print();
    b2.getAuthor().print();

    return 0;
}
