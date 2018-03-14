#include<iostream>

using namespace std;

class B1 {
    public:
        int b1;
        
        B1(int n) {
            b1=n;
            cout<<"Constructing B1"<<endl;
        }

        ~B1(){
            cout<<"Destructing B1"<<endl;
        }
};

class D1 : public B1{
    public:
        int d1;
        
        D1(int n) : B1(n+1) {
            d1=n;
            cout<<"Constructing D1"<<endl;
        }

        ~D1(){
            cout<<"Destructing D1"<<endl;
        }
};

class D2 : public D1{
    public:
        int d2;

        D2(int n) : D1(n+1) {
            d2=n;
            cout<<"Constructing D2"<<endl;
        }


        ~D2(){
            cout<<"Destructing D2"<<endl;
        }

};



int main()
{
    D2 obj(5);
    return 0;
}
