#include<iostream>

using namespace std;

class B1 {
    public:
        B1(){
            cout<<"Constructing B1"<<endl;
        }

        ~B1(){
            cout<<"Destructing B1"<<endl;
        }
};

class B2 {
    public:
        B2(){
            cout<<"Constructing B2"<<endl;
        }

        ~B2(){
            cout<<"Destructing B2"<<endl;
        }
};

class B3{
    public:
        B3(){
            cout<<"Constructing B3"<<endl;
        }

        ~B3(){
            cout<<"Destructing B3"<<endl;
        }
};


class D1:public B1, public B2, public B3{

    public:
        D1():B3(), B2(),B1() {	
            cout<<"constructing D1"<<endl;
        }

        ~D1(){
            cout<<"Destructing D1"<<endl;
        }

};






int main()
{
    D1 obj;
    return 0;
}
