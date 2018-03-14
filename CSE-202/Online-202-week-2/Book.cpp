#include <string>
#include "Author.cpp"
using namespace std;

class Book {
private:
   string name;
   Author author;
   double price;
   int stock;
public:
    Book(string name, Author author);
    Book(string name, Author author, double price, int stock);
    void setPrice(double price);
    void setStock(int stock);
    void borrowBook(int stock);
    void returnBook(int stock);
    string getAuthorName();
    string getName();
    Author getAuthor();
    int getStock();
    double getPrice();
    void print();
};

Book::Book(string name, Author author){
    this-> name=name;
    this-> author=author;
}
Book::Book(string name, Author author, double price, int stock){
    this-> name=name;
    this-> author=author;
    this-> price=price;
    this-> stock=stock;
}
void Book::setPrice(double price){
    this-> price=price;
}
void Book::setStock(int stock){
    this-> stock=stock;
}
void Book::borrowBook(int stock1){
    if(stock1>stock){
        cout<<"Borrowing amount exceeds the current stock!"<<endl;
    }
    else stock-=stock1;
}
void Book::returnBook(int stock1){
    stock+=stock1;
}
string Book::getAuthorName(){
    return this-> author.getName();
}
string Book::getName(){
    return this->name;
}
int Book::getStock(){
    return this-> stock;
}
double Book::getPrice(){
    return this-> price;
}

void Book::print(){
    cout << "Author: " << author.getName() <<" Name: " << name << " Price: " << price << " Stock: " << stock << endl;
}

Author Book::getAuthor(){
    return this->author;
}
