#include <iostream>
#include <cmath>

class Vector{

  int length;
  double** p_top;

  public:
    Vector(int i=1);
    Vector(const Vector &cp);

    ~Vector();

    int size(){return(length);}

    void change_size(int i);
    void print();
    double* &operator[](int i){return(p_top[i]);}
    Vector operator=(const Vector &a);
    Vector operator+(const Vector &a);
    Vector operator-(const Vector &a);
    friend Vector operator*(const Vector &a, double b);
    friend Vector operator*(double b, const Vector &a);
};


Matrix::Matrix(int i)
{
  if(i<1){
    std::cerr << "err Vector::Vector" << std::endl;
    exit(1);
  }

  length = i ;

  p_top = new double*[row+1];
  *p_top = new double[row*column+1];
  for(int k=1;k<=row;k++){
    *(p_top+k) = *p_top + ((k-1)*column);
  }
  for(int k1=1;k1<=row;k1++){
    for(int k2=1;k2<=column;k2++){
      p_top[k1][k2] = 0;
    }
  }
}


Matrix::Matrix(const Matrix &cp){
  row = cp.row;
  column = cp.column;

  p_top = new double*[row+1];
  *p_top = new double[row*column+1];
  for(int k=1; k<=row; k++){
    *(p_top+k) = *p_top + ((k-1)*column);
  }

  for(int k1=1; k1<=row; k1++){
    for(int k2=1; k2<column; k2++){
      p_top[k1][k2] = cp.p_top[k1][k2];
    }
  }
}


Matrix::~Matrix(){
  delete [] *p_top;
  delete [] p_top;
}

void Matrix::change_size(int i,int j){
  if(i<1 || j<1){
    std::cerr << "err Matrix::change_size" << std::endl;
    exit(1);
  }
  delete [] *p_top;
  delete [] p_top;

  row = i;
  column = j;

  p_top = new double*[row+1];
  *p_top = new double[row*column+1];

  for(int k=1; k<=row; k++){
    *(p_top+k) = *p_top + ((k-1)*column);
  }

  for(int k1=1; k1<=row; k1++){
    for(int k2=1; k2<=column; k2++){
      p_top[k1][k2] = 0;
    }
  }
}


void Matrix::print(){
  printf("[");
  for(int i=1; i<=row; i++){
    if(i!=1){printf("\n");}
    printf("[");
    for(int j=1; j<=column; j++){
      printf("%f",p_top[i][j]);
      printf(" ");
    }
    printf("]");
  }
  printf("]\n");
}

Matrix Matrix::operator=(const Matrix &a){
  if(row != a.row || column!=a.column){
    change_size(a.row,a.column);
  }

  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      p_top[i][j] = a.p_top[i][j];
    }
  }
  return(*this);
}



Matrix Matrix::operator+(const Matrix &a){
  if(row != a.row || column != a.column){
    std::cerr << "err Matrix::operator+" << std::endl;
    std::cerr << " not equal matrix size" << std::endl;
    exit(0);
  }
  Matrix r(row,column);
  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      r.p_top[i][j] = p_top[i][j] + a.p_top[i][j];
    }
  }
  return(r);
}

Matrix Matrix::operator-(const Matrix &a){
  if(row != a.row || column != a.column){
    std::cerr << "err Matrix::operator-" << std::endl;
    std::cerr << " not equal matrix size" << std::endl;
  }

  Matrix r(row,column);
  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      r.p_top[i][j] = p_top[i][j] - a.p_top[i][j];
    }
  }
  return(r);
}


Matrix Matrix::operator*(const Matrix &a){
  if(column != a.row){
    std::cerr << "err Matrix::operator*" << std::endl;
    std::cerr << " not equal matrix size" << std::endl;
    exit(0);
  }

  Matrix r(row, a.column);
  for(int i=1;i<=row; i++){
    for(int j=1;j<a.column; j++){
      for(int k=1; k<column; k++){
        r.p_top[i][j] += p_top[i][k] * a.p_top[k][j];
      }
    }
  }
  return(r);
}


Matrix operator*(const Matrix &a, double b)
{
  Matrix r(a.row, a.column);
  for(int i=1; i<=a.row; i++){
    for(int j=1; j<=a.column; j++){
      r[i][j] = b * a.p_top[i][j];
    }
  }
  return(r);
}

Matrix operator*(double b, const Matrix &a)
{
  Matrix r(a.row, a.column);
  for(int i=1; i<=a.row; i++){
    for(int j=1; j<=a.column; j++){
      r[i][j] = b * a.p_top[i][j];
    }
  }
  return(r);
}

Vector operator*(const Matrix &a, Vector b)
{
  if(a.column != b.size()){
    std::cerr << "err operator*" << std::endl;
    std::cerr << " size of matrix and vector do not match" << std::endl;
    exit(0);
  }
  Vector r(a.row);
  for(int i=1; i<=a.row; i++){
    for(int j=1; j<=a.column; j++){
      r.set(i-1, r.get(i-1) + a.p_top[i][j] * b.get(j-1));
    }
  }
  return(r);
}



void Matrix::unit_matrix()
{
  if(row != column){
    std::cerr <<"err in Matrix::unit_matrix()" << std::endl;
    exit(0);
  }

  int n = row;
  double** a = p_top;
  for(int i=1; i<=n; i++){
    for(int j=1; j<=n; j++){
      a[i][j] = 0;
      if(i == j) a[i][j] = 1;
    }
  }

}

Matrix Matrix::transpose()
{
  Matrix t(column, row);
  double** a = p_top;

  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      t[j][i] = a[i][j];
    }
  }
  return(t);
}

/*
int main(void){
  Matrix a(3,3);
  a.unit_matrix();
  a.print();
}*/
