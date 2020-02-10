#include "BicTheta.h"
#include <iostream>
using namespace Rcpp ;
using namespace Eigen;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP BicTheta(SEXP X,SEXP Y,SEXP beta,SEXP intercept){
BEGIN_RCPP
  
  const Map<MatrixXd> vecY(as<Map<MatrixXd> >(Y));//Y
  Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<MatrixXd> Beta(as<Map<MatrixXd> >(beta));//beta

  Rcpp::NumericVector xintercept(intercept);;
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator iinter=xintercept.begin();;
  int size = matX.rows();
  int sizecol=matX.cols();
  
  Eigen::MatrixXd temp(size,sizecol+1);
  
  
  //ajout de la constance si demande
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
    if (temp.cols() != Beta.rows()){
      throw std::range_error("beta n'est pas de la meme dimension que X");
    }else{
    Eigen::MatrixXd residus = temp*Beta;
    Eigen::MatrixXd sig = (vecY-residus);
    //calcul de sigma
    float sumc=0;
    for (int i = 0; i < sig.rows(); i++){
        sumc=sumc+(sig(i,0)-sig.mean())*(sig(i,0)-sig.mean());
      }
      
    sumc=sumc/(sig.rows()-1);
    sumc=sqrt(sumc);

    //calcul du BIC
    int k=1;
    for (int i = 0; i < Beta.rows(); i++){
      if (Beta(i,0)!=0){
        k=k+1;
      }
    }
    Eigen::MatrixXd Sum=sig.transpose()*sig;
    float nbsum=Sum(0,0);
        
    float log_likelihood = (-vecY.rows()/2)*log(2*PI*sumc*sumc)-nbsum/(2*sumc*sumc);
        
    float BicTheta=(-2*log_likelihood+(k*log(float(vecY.rows()))));
  
  
    return List::create(
      Named("BIC")=  BicTheta
    );
    }
END_RCPP
}
