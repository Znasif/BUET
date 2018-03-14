#include<iostream>

using namespace std;

class B1 {
    private:
        int a;
    public:
	B1(){
            cout<<"Constructing B1"<<endl;
        }

        void setA(int n){
            a=n;
        }

        void getA(){
            cout<<a<<endl;
        }

};


class D1: virtual public B1{
      public:

        D1(){
            cout<<"Constructing D1"<<endl;
        }
        void setA(int n){
            B1::setA(n*2);
        }

};

class D2: virtual public B1{
    public:
        D2(){
            cout<<"Constructing D2"<<endl;
        }
        void setA(int n){
            B1::setA(n*3);
        }

};

class D3: public D1, public D2{
    public:
        D3(){
            cout<<"Constructing D3"<<endl;
        }
        void setA(int n){
            B1::setA(n*4);
        }

};

int main(){

    D3 obj3;
    D2 obj2;
    D1 obj1;
    B1 obj;
//    obj.setA(100);
//    obj.getA();
//    obj1.setA(100);
//    obj1.getA();
//    obj2.setA(100);
//    obj2.getA();
//    obj3.setA(100);
//    obj3.getA();
    return 0;
}
