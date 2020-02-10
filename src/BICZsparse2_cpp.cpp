#include "BICZsparse2_cpp.h"
#include "BicLoc_cpp.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;


Eigen::VectorXd BICZsparse2_cpp(Eigen::MatrixXd X,Rcpp::NumericVector Z_Zi,Eigen::VectorXd Z_Sj,Eigen::VectorXd Bic_vide_vect,int methode,int nrow)
{
  Eigen::VectorXd BIC_vect;
  int debut_colj;
  Eigen::MatrixXd X_loc;
  int m;
  int size = X.cols();
  
  BIC_vect=Bic_vide_vect;
  
  for(int k=0;k<size;k++)//pour chaque colonne
  {
    if(Z_Sj(k)!=0)
    {
      //on créé X_loc et y_loc
      X_loc.resize(nrow,(Z_Sj(k)));
        
      debut_colj=0;
      for(m=0;m<k;m++)//on regarde ou commence la colonne j
      {
        debut_colj=debut_colj+Z_Sj(m);
      }
      for(m=0;m<Z_Sj(k);m++)
      {
        X_loc.block(0,m,nrow,1)=X.block(0,Z_Zi(debut_colj+m)-1,nrow,1);
      }
      //on calcul le BIC
      BIC_vect(k)=BicLoc_cpp(X_loc,X.block(0,k,nrow,1),"T",methode);
    }
  }
  return BIC_vect;
}
  
