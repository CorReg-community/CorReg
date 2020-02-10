#include "BicLoc_cpp.h"
#include <iostream>
#include <string>


using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

double BicLoc_cpp(const Eigen::MatrixXd &X,const Eigen::MatrixXd &Y,const bool &intercept,const int &methode){
  
  int size = X.rows();
  int sizecol=X.cols();
  
  Eigen::MatrixXd A;
  Eigen::MatrixXd temp(size,sizecol+1);
  double BicTheta;
  double sumc=0;
  //ajout du vecteur de 1
  if (intercept==1){
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
    
    //retour de sigma et beta avec calcul du sigma
      
      Eigen::MatrixXd residus = temp*inverse;
      Eigen::MatrixXd sig = (Y-residus);
      
      //calcul de ecart type (sigma)
      for (int i = 0; i < sig.rows(); i++){
        sumc=sumc+(sig(i,0)-sig.mean())*(sig(i,0)-sig.mean());
      }
      
      sumc=sumc/(sig.rows()-1);
      sumc=sqrt(sumc);
      if(sumc==0){//modele exact
        BicTheta=0;
       Rcpp::Rcout<<"exact model found, please delete one variable \n";
      }else{
      //calcul du BIC
        int k=inverse.rows()+1;
        Eigen::MatrixXd Sum=sig.transpose()*sig;
        double nbsum=Sum(0,0);
        
        double log_likelihood = (-Y.rows()/2)*log(2*PI*sumc*sumc)-nbsum/(2*sumc*sumc);
        BicTheta=(-2*log_likelihood+(k*log(double(Y.rows()))));
      }   
return BicTheta;
}
