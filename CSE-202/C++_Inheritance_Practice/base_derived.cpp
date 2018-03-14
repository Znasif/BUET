#include<iostream>

using namespace std;

class base {
    private:
        int x;

    protected:
        int y;

    public:
        int z;

        void setX(int n){
            x=n;
        }

        void setY(int n){
            y=n;
        }
};

class derived: private base{
    public:
        void setY(int n){
            y=n;
        }

        int getY()
        {
            return y;
        }
};

int main() {
    derived obj;
    obj.setY(5);
    //obj.x = 10;
    //obj.z = 20;
    cout << obj.getY() << endl;
    return 0;
}
