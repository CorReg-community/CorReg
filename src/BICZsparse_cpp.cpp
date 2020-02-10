#include "BICZsparse_cpp.h"
#include "BicLoc_cpp.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Rcpp::as;

Eigen::VectorXd BICZsparse_cpp(Eigen::MatrixXd X,Rcpp::NumericVector Z_Zi,Eigen::VectorXd Z_Sj,Eigen::VectorXd Bic_vide_vect,Eigen::VectorXd BicOld,int methode,Eigen::VectorXd Zold_Sj,int nrow)
{
  Eigen::VectorXd BIC_vect;
  int debut_colj;
  Eigen::MatrixXd X_loc;
  int m;
  int size = X.cols();
  
  
  BIC_vect=BicOld;
//  cout<<"size : "<<size<<"\n";
  
  for(int k=0;k<size;k++)//pour chaque colonne
  {
//    cout<<k;
    if(Z_Sj(k)!=Zold_Sj(k))//si on a change qqchose
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
//        cout<<"ici6 \n";
//        cout<<"size : "<< Z_Zi.size()<<"\n";
//        cout<<"z_sj(k):"<<Z_Sj(k)<<"\n";
        for(m=0;m<Z_Sj(k);m++)//création de la matrice X suivant les dependances
        {
          
//         cout<<"debut col : "<<debut_colj<<"\n";
//        cout<<"m: "<<m<<"\n";
//         
//         cout<<Z_Zi(debut_colj+m)-1;
          
          X_loc.block(0,m,nrow,1)=X.block(0,Z_Zi(debut_colj+m)-1,nrow,1);
        }
//        cout<<"bicloc \n";
        //cout<<X_loc;
        //on calcul le BIC
        BIC_vect(k)=BicLoc_cpp(X_loc,X.block(0,k,nrow,1),"T",methode);
//        cout<<"ici";
      }
      else //colonne vide
      {
//        cout<<"ici2";
        BIC_vect(k)=Bic_vide_vect(k);
       
      }
    }
  }
  return BIC_vect;
}
  
