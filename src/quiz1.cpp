// https://mementoo.info/archives/648

#include <iostream>
#include <stdio.h>
#include <string.h>

//#include "vector.cpp"

#include "matrix.cpp"


int main(void){
  printf("========== Quiz 1 ==========\n");
  // ベクトル1の定義
  Vector v1(3);
  for(int i=0;i<3;i++){
    v1.set(i,i+2);
  }
  printf("\n========== Vector1 ==========\n");
  v1.print();


  // ベクトル2の定義
  Vector v2(3);
  for(int i=0;i<3;i++){
    v2.set(i,2*i-1);
  }
  printf("\n========== Vector2 ==========\n");
  v2.print();

  // 行列の定義
  Matrix m1(3,3);
  m1[1][1] = 1.0;
  m1[1][2] = 2.0;
  m1[2][1] = 3.0;
  m1[2][2] = 1.0;
  printf("\n========== Matrix1 ==========\n");
  m1.print();

  // ベクトルの足し算
  Vector v3 = v1 + v2;
  printf("\n========== Vector1 + Vector2 ==========\n");
  v3.print();

  Vector v4 = m1 * v1;
  printf("\n========== Matrix1 * Vector1 ==========\n");
  v4.print();

  Matrix m2 = m1.transpose();
  printf("\n========== transpose of Matrix1 ==========\n");
  m2.print();

  Vector v5 = ReLU(v2);
  printf("\n========== ReLU(Vector2) ==========\n");
  v5.print();

  Vector v6 = Softmax(v2);
  printf("\n========== Softmax(Vector2) ==========\n");
  v6.print();


}
