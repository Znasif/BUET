#include<iostream>

using namespace std;

class shape {
    protected:
        int height;
        int width;

    public:
        void setValues(int h, int w){
            height=h;
            width=w;
        }

        /*int getArea() {
		return height*width;
	}*/
	/*virtual int getArea() {
		return height*width;
	}*/
	virtual int getArea() = 0;
};

class rectangle: public shape{
    public:
        int getArea() {
            return height*width;
        }
};

class triangle:public shape{
    public:
        int getArea(){
            return height*width/2;
        }
};

int main()
{

    rectangle rect;
    triangle tri;
    shape *sample;
    sample=&rect;
    sample->setValues(10,20);
    cout<<sample->getArea()<<endl;
    sample=&tri;
    sample->setValues(10,20);
    cout<<sample->getArea()<<endl;
    return 0;
}
