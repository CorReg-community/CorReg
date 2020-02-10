#include "OLS.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace Rcpp ;
using namespace Eigen;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP OLS(SEXP X,SEXP Y,SEXP intercept,SEXP sigma,SEXP Bic,SEXP methode){
 
  BEGIN_RCPP
  
  const Map<MatrixXd> vecY(as<Map<MatrixXd> >(Y));//Y
  Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  
  Rcpp::NumericVector xmet(methode),xinter(intercept),xsigma(sigma),xBic(Bic);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imet = xmet.begin(),iinter=xinter.begin(),isigma=xsigma.begin(),iBic=xBic.begin();
  int size = matX.rows();
  int sizecol=matX.cols();
  
  Eigen::MatrixXd A;
  Eigen::MatrixXd temp(size,sizecol+1);
  float BicTheta;
  float sumc=0;
  //ajout du vecteur de 1
  if (iinter[0]==1){
    Eigen::ArrayXd vec1(size); 
     
    for(int i=0;i<=size-1;i++){ 
      vec1(i)=1;
    }  
    temp<<vec1,matX;
    }else{
      temp.resize(size,size);
      temp=matX;
    }
      
    
    Eigen::MatrixXd inverse;
  
  //choix du calcul de beta
    if (imet[0]==1){
      inverse = temp.householderQr().solve(vecY);
    }else{ 
      if (imet[0]==2){
        inverse = temp.colPivHouseholderQr().solve(vecY);
      }else{
//        if(imet[0]==3){
//          inverse = temp.ldlt().solve(vecY);  
//        }else{
          inverse = temp.householderQr().solve(vecY);
//        }
      }
    }
  
    //retour de sigma et beta avec calcul du sigma
    if(isigma[0]==1 || iBic[0]==1){
      
      Eigen::MatrixXd residus = temp*inverse;
      Eigen::MatrixXd sig = (vecY-residus);
      
      //calcul de l'ecart type (sigma)
      for (int i = 0; i < sig.rows(); i++){
        sumc=sumc+(sig(i,0)-sig.mean())*(sig(i,0)-sig.mean());
      }
      
      sumc=sumc/(sig.rows()-1);
      sumc=sqrt(sumc);
      
      //calcul du BIC
      if(iBic[0]==1){
        int k=inverse.rows()+1;
        Eigen::MatrixXd Sum=sig.transpose()*sig;
        float nbsum=Sum(0,0);
        
        float log_likelihood = (-vecY.rows()/2)*log(2*PI*sumc*sumc)-nbsum/(2*sumc*sumc);
        
        BicTheta=(-2*log_likelihood+(k*log(float(vecY.rows()))));
      }

  }
  //retour des objets demandes
  if (isigma[0]==1){
    if (iBic[0]==1){
          return List::create(
          Named("beta")=  inverse,
          Named("sigma")=  sumc,
          Named("BIC")=  BicTheta
        );
    }else{
          return List::create(
          Named("beta")=  inverse,
          Named("sigma")=  sumc
        );
    }
  }else{
    if(iBic[0]==1){
          return List::create(
          Named("beta")=  inverse,
          Named("BIC")=  BicTheta
          );
    }else{
          return List::create(
          Named("beta")=  inverse
          );
    }
  }
  
  END_RCPP
}
