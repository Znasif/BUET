#include <iostream>
#include <string>
using namespace std;

class Author {
private:
   string name;
   string email;
public:
   Author();
   Author(string name);
   Author(string name, string email);
   string getEmail();
   string getName();
   void setEmail(string email);
   void print();
};

Author::Author() {

}

Author::Author(string name) {
    this-> name=name;
}

Author::Author(string name, string email) {
    this-> name=name;
    this-> email=email;
}

string Author::getName(){
    return this-> name;
}

string Author::getEmail(){
    return this-> email;
}

void Author::setEmail(string email){
    this-> email=email;
}
void Author::print(){
    cout << name <<  " at " << email << endl;
}
