// https://mementoo.info/archives/648

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream> // ifstream
#include <sstream> // stringstream
using namespace std;

#include "matrix.cpp"



int main() {
   int row = 0, col = 0, numrows = 0, numcols = 0;
   ifstream infile("../pgm/1.pgm");
   stringstream ss;
   string inputLine = "";

   // First line : version
   getline(infile,inputLine);
   if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
   else cout << "Version : " << inputLine << endl;

   // Second line : comment
   getline(infile,inputLine);
   cout << "Comment : " << inputLine << endl;

   // Continue with a stringstream
   ss << infile.rdbuf();
   // Third line : size
   ss >> numcols >> numrows;
   cout << numcols << " columns and " << numrows << " rows" << endl;

   int array[numrows][numcols];

   // Following lines : data
   for(row = 0; row < numrows; ++row)
     for (col = 0; col < numcols; ++col) ss >> array[row][col];

   // Now print the array to see the result
   for(row = 0; row < numrows; ++row) {
     for(col = 0; col < numcols; ++col) {
       cout << array[row][col] << " ";
     }
     cout << endl;
   }
   infile.close();
 }
