// https://mementoo.info/archives/648

#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

class Vector{
  //private:
  //xdVec(){} これ？？
  double *value;
  int length;

  public:
    Vector(int length);
    Vector(const Vector& v);

    ~Vector();

    void set(int index,double value);
    // 特に必要はない
    double get(int index);
    // 特に必要はない
    int size();

    Vector operator+(const Vector& v);
    
    void print();

    int argmax();

    // なんかこれも使われてないっぽい。。。なんだろう
    Vector& operator=(const Vector& v);
};

Vector::Vector(int length){
  this->length = length;
  value = new double[length];
  memset(value,0,length*sizeof(double));
}

    // なんか来れなくてもうまくいく。。。何者なんだ
Vector::Vector(const Vector& v){
  length = v.length;
  value = new double[length];
  memcpy(value,v.value,length*sizeof(double));
}

Vector::~Vector(void){
  delete[] value;
  value = NULL;
}

void Vector::set(int index,double value){
  this->value [index] = value;
}

    // 特に必要はない
double Vector::get(int index) {return value[index];}
    // 特に必要はない
int Vector::size(void) {return length;}

Vector Vector::operator+(const Vector& v) {
  Vector dst(length);
  for(int i=0;i<length;i++){
    dst.value[i] = value[i] + v.value[i];
  }
  return dst;
}

void Vector::print(void) {
  printf("[");
  for(int i=0;i<length;i++){
    printf("%f",value[i]);
    if(i<length-1){printf(" ");}
  }
  printf("]\n");
}

int Vector::argmax(void){
  int argmax = 0;
  for(int i=0; i<length; i++){
    if(value[argmax]<value[i]){
      argmax = i;
    }
  }
  return argmax;
}

  // なんかこれも使われてないっぽい。。。なんだろう
Vector& Vector::operator=(const Vector& v){
  delete[] value;
  length = v.length;
  value = new double[length];
  memcpy(value,v.value,length*sizeof(double));
  return *this;
}


Vector ReLU(Vector v1){
  int length = v1.size();
  Vector r(length);
  for(int i=0; i<length; i++){
    if(v1.get(i)>0){r.set(i,v1.get(i));}
    else{r.set(i,0);}
  }
  return(r);
}

double exp(double x){
  return pow(2.71828182846,x);
}

Vector Softmax(Vector v1){
  int length = v1.size();
  Vector r(length);
  double sum=0;
  for(int i=0; i<length; i++){
    sum += exp(v1.get(i));
  }
  for(int i=0; i<length; i++){
    r.set(i,exp(v1.get(i))/sum);
  }
  return(r);
}
