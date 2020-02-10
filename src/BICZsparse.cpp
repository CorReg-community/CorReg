#include "BICZsparse.h"
#include "BICZsparse_cpp.h"

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

SEXP BICZsparse(SEXP X,SEXP Z_Zi,SEXP Z_Sj,SEXP Bic_vide_vect,SEXP BicOld,SEXP methode,SEXP Zold_Sj,SEXP nrow)
{
  BEGIN_RCPP
  const Map<VectorXd> Z_zi(as<Map<VectorXd> >(Z_Zi));//Z
  const Map<VectorXd> Z_sj(as<Map<VectorXd> >(Z_Sj));//Z
  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<VectorXd> Zold_sj(as<Map<VectorXd> >(Zold_Sj));//Zold
  const Map<VectorXd> bicold(as<Map<VectorXd> >(BicOld));//X
  const Map<VectorXd> vide(as<Map<VectorXd> >(Bic_vide_vect));//Bic_vide_vect
  
  Rcpp::NumericVector Methode(methode),Nrow(nrow);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imethode = Methode.begin(),inrow=Nrow.begin();
  Eigen::VectorXd BIC_vect;
  int debut_colj;
  Eigen::MatrixXd X_loc;
  int m;
  int size = matX.cols();
  
  BIC_vect=bicold;

  for(int k=0;k<size;k++)//pour chaque colonne
  {
    if(Z_sj(k)!=Zold_sj(k))//si on a change qqchose
    {
      if(Z_sj(k)!=0)
      {
        //on fait X_loc et y_loc
        X_loc.resize(inrow[0],(Z_sj(k)));
        
        debut_colj=0;
        for(m=0;m<k;m++)//on regarde ou commence la colonne j
        {
          debut_colj=debut_colj+Z_sj(m);
        }
        for(m=0;m<Z_sj(k);m++)
        {
          X_loc.block(0,m,inrow[0],1)=matX.block(0,Z_zi(debut_colj+m)-1,inrow[0],1);
        }
        
        //on calcul le BIC
        BIC_vect(k)=BicLoc_cpp(X_loc,matX.block(0,k,inrow[0],1),"T",imethode[0]);
      }
      else //colonne vide
      {
        BIC_vect(k)=vide(k);
      }
    }
  }
  return List::create(
      Named("BIC")=  BIC_vect
    );
  END_RCPP
}
  
