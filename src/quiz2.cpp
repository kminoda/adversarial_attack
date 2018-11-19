// https://teratail.com/questions/141589

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <string>
using namespace std;

#define MAP_HEIGHT 32
#define MAP_WIDTH 32

#include "util.cpp"

int main(){
  cout << "==================start==================" << endl;
  cout << "  file name    " << "pred\t" << "ans\t" << "T/F" << endl;
  cout << "---------------------------------------" << endl;

  // パラメータ取得
  Matrix W1,W2,W3;
  Vector b1(256),b2(256),b3(23);

  tie(W1,W2,W3,b1,b2,b3) = get_params();
  // ラベル取得
  Vector labels(154);
  labels = get_labels();

  // ファイル名作成
  int length = 154;
  string name_list[length];
  for(int i=0; i<length; i++){
    name_list[i] = "./pgm/" + to_string(i+1) + ".pgm";
  }

  int num_correct = 0;
  for(int i=0; i<length; i++){
    // パラメータを用いてラベル予測
    Vector x(1024),a1(256), h1(256), a2(256), h2(256), y(23);
    x = matrix_to_vector(get_matrix(name_list[i]));
    a1 = W1*x + b1;
    h1 = ReLU(a1);
    a2 = W2*h1 + b2;
    h2 = ReLU(a2);
    y = Softmax(W3*h2 + b3);
    //y = f(x,b1,b2,b3,W1,W2,W3);
    int pred = y.argmax()+1;

    // 正解ラベルとともに出力。正解していたらカウントを増やす。
    int label = labels[i];
    if(pred==label) num_correct++;
    cout << name_list[i] << ": \t" << pred << "\t" << label << "\t" << (pred==label) << endl;
  }


  cout << "=======================================" << endl;

  cout << "accuracy = " << num_correct/1.54 << "%" <<  endl;

  cout << "==================end==================" << endl;
}
