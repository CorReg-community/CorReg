#include "OLS_cpp.h"
#include <iostream>


using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

MatrixXd OLS_cpp(const MatrixXd &X,const MatrixXd &Y,const bool &intercept,const int &methode){
  
  int size = X.rows();
  int sizecol=X.cols(); 
  
  Eigen::MatrixXd A;
  Eigen::MatrixXd temp(size,sizecol+1);
  //ajout du vecteur de 1
  if (intercept){
    Eigen::ArrayXd vec1(size); 
     
    for(int i=0;i<=size-1;i++){ 
      vec1(i)=1;
    }  
    temp<<vec1,X;
    }else{
      temp.resize(size,size);
      temp=X;
    }
     
    
    Eigen::MatrixXd inverse;
  
  //choix du calcul de beta
    if (methode==1){
      inverse = temp.householderQr().solve(Y);
    }else{ 
      if (methode==2){
        inverse = temp.colPivHouseholderQr().solve(Y);
      }else{
          inverse = temp.householderQr().solve(Y);
      }
    }
  return inverse;
}
