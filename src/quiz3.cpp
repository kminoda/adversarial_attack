// https://teratail.com/questions/141589

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <string>
# include <random>
using namespace std;

#define MAP_HEIGHT 32
#define MAP_WIDTH 32

#include "util.cpp"

int main(){
  cout << "======================start======================" << endl;
  cout << "  file name    " << "ans\t" << "pred\t" << "FGSM\t" << "Random" << endl;
  cout << "-------------------------------------------------" << endl;

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

  int num_correct_1 = 0;
  int num_correct_2 = 0;
  int num_correct_3 = 0;


  for(int i=0; i<length; i++){
  //for(int i=0; i<1; i++){

    Vector x(1024),a1(256), h1(256), a2(256), h2(256), fx(23);
    Vector x_tilde(1024),a1_tilde(256), h1_tilde(256), a2_tilde(256), h2_tilde(256), fx_tilde(23);
    Vector x_random(1024),fx_random(23);
    int label,pred_1,pred_2,pred_3;
    Vector NxL(1024),Na1L(256),Nh1L(256),Na2L(256),Nh2L(256),NyL(23),signNxL(1024);
    double eps_0 = 0.1;
    Vector eps_FGSM(1024);
    Vector eps_random(1024);
    // 正解ラベルとともに出力。正解していたらカウントを増やす
    label = labels[i];

    // パラメータを用いてラベル予測
    x = matrix_to_vector(get_matrix(name_list[i]));
    a1 = W1*x + b1;
    h1 = ReLU(a1);
    a2 = W2*h1 + b2;
    h2 = ReLU(a2);
    fx = Softmax(W3*h2 + b3);
    pred_1 = fx.argmax()+1;


    Matrix m(2,2);
    m[0][0] = 0.0;
    m[0][1] = 1.2;
    m[1][0] = 0.8;
    m[1][1] = 0.0;

    Vector v(2);
    v[0] = 0.5;
    v[1] = 0.6;

    (m.transpose()*v).print();

    // FGSMによる擾乱を加えてラベル予測
    Vector dt(23);
    dt[labels[i]-1] = 1.0;
    NyL = fx - dt;
    Nh2L = W3.transpose() * NyL;
    Na2L = Backward(Nh2L,a2);
    Nh1L = W2.transpose() * Na2L;
    Na1L = Backward(Nh1L,a1);
    NxL = W1.transpose() * Na1L;
    signNxL = sign(NxL);
    eps_FGSM = signNxL * eps_0;
    //signNxL.print();
    x_tilde = x + eps_FGSM;

    a1_tilde = W1*x_tilde + b1;
    h1_tilde = ReLU(a1_tilde);
    a2_tilde = W2*h1_tilde + b2;
    h2_tilde = ReLU(a2_tilde);
    fx_tilde = Softmax(W3*h2_tilde + b3);
    pred_2 = fx_tilde.argmax()+1;

    // ランダムな擾乱を加えてラベル予測
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> score(-1.0,1.0);
    for(int i=0; i<1024; ++i){
      eps_random[i] = score(mt)*eps_0;
    }
    x_random = x + eps_random;
    a1 = W1*x_random + b1;
    h1 = ReLU(a1);
    a2 = W2*h1 + b2;
    h2 = ReLU(a2);
    fx_random = Softmax(W3*h2 + b3);
    pred_3 = fx_random.argmax()+1;


    // 3つの結果を書き出し
    if(pred_1==label) num_correct_1++;
    if(pred_2==label) num_correct_2++;
    if(pred_3==label) num_correct_3++;
    cout << name_list[i] << ": \t" << label << "\t" << pred_1 << "\t" << pred_2 << "\t" << pred_3 << "\t" << endl;
  }

  cout << "-------------------------------------------------" << endl;

  cout << "Normal: " << num_correct_1/1.54 << "%" <<  endl;
  cout << "FGSM disturbance: " << num_correct_2/1.54 << "%" <<  endl;
  cout << "Random disturbance: " << num_correct_3/1.54 << "%" <<  endl;


  cout << "=======================end=======================" << endl;
}
