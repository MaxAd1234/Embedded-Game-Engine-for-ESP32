#pragma once
#include <cmath>
template <typename T>
// basic 2 dimensional vector 
class Vector
{
private:
    T x;
    T y;
public:
    Vector(T xValue, T yValue){
    x = xValue;
    y = yValue;
}
    Vector(){
    x = 0;
    y = 0;
}
    
    T getY(){
    return y;
}
    T getX(){
    return x;
}
    void setY(T yValue){
    y = yValue;
}
    void setX(T xValue){
    x = xValue;
}
    float getLength(){
    return sqrt((x*x)+(y*y));
}
    void normalize(){
    if(this->getLength() == 0){
        return;
    }
    x = x/getLength();
    y = y/getLength();
}
};


