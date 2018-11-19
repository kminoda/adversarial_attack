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

#include "matrix.cpp"

Matrix get_matrix(string name){
  ifstream stream(name);
  string line;
  const string delim = " ";

  int row = 0;
  int col;
  int count = 0;

  Matrix m1(32,32);
  while ( getline(stream, line) ) {
    if(count==0||count==1||count==2){
    }else{
      col = 0;
      // delimを区切り文字として切り分け、intに変換してdata[][]に格納する
      for ( string::size_type spos, epos = 0;
          (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
            string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
            m1[row+1][col+1] = stoi(token);
            col++;
          }
      ++row;
    }
    ++count;
  }
  return(m1);
}


Vector matrix_to_vector(Matrix m){
  int row = m.row_size();
  int column = m.column_size();

  Vector v(row*column);

  for(int i=1;i<=row;i++){
    for(int j=1;j<=column;j++){
      v.set((i-1)*32+j-1, m[i][j]/255);
    }
  }
  return(v);
}


Vector get_labels(){
  ifstream stream("./labels.txt");
  string line;
  const string delim = "\n";

  Vector labels(154);
  int col = 0;
  while ( getline(stream, line) ) {
    labels.set(col,stoi(line.c_str()));
    col++;
  }
  return labels;
}

std::tuple<Matrix,Matrix,Matrix,Vector,Vector,Vector> get_params(){
  ifstream stream("./param.txt");
  string line;
  const string delim = " ";

  Matrix W1(256,1024);
  Matrix W2(256,256);
  Matrix W3(23,256);

  Vector b1(256);
  Vector b2(256);
  Vector b3(23);

  int row = 0;
  int col;

  while ( getline(stream, line) ) {
    // W1に格納
    if(row <= 255){
      col = 0;
      for ( string::size_type spos, epos = 0;
          (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
            string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
            W1[row+1][col+1] = stod(token);
            col++;
          }
    // b1に格納
    }else if(row == 256){
      col = 0;
      for ( string::size_type spos, epos = 0;
          (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
            string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
            b1.set(col,stod(token));
            col++;
          }
    // W2に格納
    }else if(row <= 512){
      col = 0;
      for ( string::size_type spos, epos = 0;
          (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
            string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
            W2[row-256][col+1] = stod(token);
            col++;
          }
    // b2に格納
    }else if(row == 513){
      col = 0;
      for ( string::size_type spos, epos = 0;
          (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
            string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
            b2.set(col,stod(token));
            col++;
          }
    // W3に格納
    }else if(row <= 536){
      col = 0;
      for ( string::size_type spos, epos = 0;
          (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
            string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
            W3[row-513][col+1] = stod(token);
            col++;
          }
    // b3に格納
    }else if(row == 537){
          col = 0;
          for ( string::size_type spos, epos = 0;
              (spos = line.find_first_not_of(delim, epos)) != string::npos;) {
                string token = line.substr(spos,(epos = line.find_first_of(delim, spos))-spos);
                b3.set(col,stod(token));
                col++;
          }
    }
    ++row;
  }
  return std::forward_as_tuple(W1,W2,W3,b1,b2,b3);
}


int main(){
  cout << "==================start==================" << endl;

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
  cout << "  file name    " << "pred\t" << "ans\t" << "T/F" << endl;
  cout << "---------------------------------------" << endl;
  // ファイル名リストの中にあるデータを加工。
  int num_correct = 0;
  for(int i=0; i<length; i++){
    Vector x(1024), h1(256), h2(256), y(23);
    x = matrix_to_vector(get_matrix(name_list[i]));
    h1 = ReLU(W1*x + b1);
    h2 = ReLU(W2*h1 + b2);
    y = Softmax(W3*h2 + b3);
    int label = labels.get(i);
    int pred = y.argmax()+1;

    cout << name_list[i] << ": \t" << pred << "\t" << label << "\t" << (pred==label) << endl;
    if(pred==label) num_correct++;
  }
  cout << "=======================================" << endl;

  cout << "accuracy = " << num_correct/1.54 << "%" <<  endl;

  cout << "==================end==================" << endl;
}
