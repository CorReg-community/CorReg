#include "BicZ_j_cpp.h"
#include "BicLoc_cpp.h"
#include <iostream>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

Eigen::VectorXd BicZ_j_cpp(Eigen::MatrixXd X,Eigen::MatrixXd Z,Eigen::VectorXd Bic_vide_vect,Eigen::VectorXd BicOld,double methode,int j)
{
  int size=X.cols();
  int sizerow = X.rows();
  Eigen::VectorXd BicRes(size);
  Eigen::MatrixXd mat_var_a_droite;
  int k;
  //debut de la boucle colonne de Z
  BicRes=BicOld;
   
  //colonne de Z sur laquelle on va travailler (variable a gauche)
  Eigen::VectorXd colZ=Z.block(0,j,size,1);
  int newsize=colZ.sum(); 
  //si la somme de la colonne=0 alors on appelle mixmod
  if(newsize==0)
  {
    BicRes(j)=Bic_vide_vect(j);
  }
  else
  {
    //calcul du nouveau BIC    
    
    mat_var_a_droite.resize(sizerow,newsize);
    k=0;
    //on echerche l'emplacement des 1 dans le vecteur colonne (variables a droite)
    for(int p=0;p<size;p++)
    {
      if (colZ(p)==1)
      {
        //on regroupe les variables a droite dans une matrice
        mat_var_a_droite.block(0,k,sizerow,1)=X.block(0,p,sizerow,1);
        k=k+1;
      }//fin if
    }//fin for j
        //calcul de son BIC en appellant OLS (les parametres sont : Y=X[,colonne],X=X[,matrice des variables à droite])
       BicRes(j)=BicLoc_cpp(mat_var_a_droite,X.block(0,j,sizerow,1),"T",methode);
  }//fin else
    return BicRes;
    
}
