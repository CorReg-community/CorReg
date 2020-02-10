#include "BICZsparse_j_cpp.h"
#include "BicLoc_cpp.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace Rcpp;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

Eigen::VectorXd BICZsparse_j_cpp(Eigen::MatrixXd X,Eigen::VectorXd Z_Zi,Eigen::VectorXd Z_Sj,Eigen::VectorXd Bic_vide_vect,Eigen::VectorXd BicOld,int methode,int nrow,int j)
{
  Eigen::VectorXd BIC_vect;
  int debut_colj;
  Eigen::MatrixXd X_loc;
  int m;
  
  BIC_vect=BicOld;
    
  if(Z_Sj(j)!=0)//la colonne a changé on calcul le BIC
  {
    X_loc.resize(nrow,(Z_Sj(j)));
    //on créé X_loc et y_loc
    debut_colj=0;
    for(m=0;m<j;m++)//on regarde ou commence la colonne j
    {
      debut_colj=debut_colj+Z_Sj(m);
    }
    for(m=0;m<Z_Sj(j);m++)
    {
      X_loc.block(0,m,nrow,1)=X.block(0,Z_Zi(debut_colj+m)-1,nrow,1);
    }
        
    //on calcul le BIC
    BIC_vect(j)=BicLoc_cpp(X_loc,X.block(0,j,nrow,1),"T",methode);
    }
    else //colonne vide
    {
      BIC_vect(j)=Bic_vide_vect(j);
    }
    return BIC_vect;  
}
  
