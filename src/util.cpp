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


Vector Backward(Vector p, Vector q){
  if(p.size() != q.size()) cerr << "err: vector size do not match." << endl;
  
  Vector r(q.size());
  for(int i=0; i<q.size(); i++){
    if(q.get(i)>0){r.set(i,p.get(i));}
    else{r.set(i,0);}
  }
  return r;
}

Vector sign(Vector x){
  Vector r(x.size());
  for(int i=0; i<x.size(); i++){
    if(x.get(i) > 0){r.set(i,1);}
    else if(x.get(i) == 0){r.set(i,0);}
    else{r.set(i,-1);}
  }
  return r;
}


Vector f(Vector x,Vector b1,Vector b2,Vector b3,Matrix W1,Matrix W2,Matrix W3){
  Vector a1(256), h1(256), a2(256), h2(256), fx(23);
  a1 = W1*x + b1;
  h1 = ReLU(a1);
  a2 = W2*h1 + b2;
  h2 = ReLU(a2);
  fx = Softmax(W3*h2 + b3);
  return fx;
}
