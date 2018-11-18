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
  using namespace std;
  ifstream stream(name);
  string line;
  int data[32][32];
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
            m1[row][col++] = stoi(token);
            data[row][col] = stoi(token);
          }
      ++row;
    }
    ++count;
  }
  return(m1);
}


int main(){
  int length = 154;
  string name_list[length];

  // ファイルの名前を作成。
  for(int i=0; i<length; i++){
    name_list[i] = "./pgm/" + to_string(i+1) + ".pgm";
  }

  // ファイルの名前リストに対応するデータを取ってきて、Matrix型として格納する。
  Matrix m_list[length];
  for(int i=0; i<length; i++){
    m_list[i] = get_matrix(name_list[i]);
  }

  // debug
  m_list[0].print();

}
