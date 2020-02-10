#include "BicZ_cpp2.h"
#include "BicLoc_cpp.h"
#include <iostream>
using namespace Rcpp ;
using namespace Eigen;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

Eigen::VectorXd BicZ_cpp2(Eigen::MatrixXd X,Eigen::MatrixXd Z,Eigen::VectorXd Bic_vide_vect,int methode){

int size = X.cols();
int sizerow = X.rows();
Eigen::VectorXd BicRes(size);
Eigen::MatrixXd mat_var_a_droite;
//debut de la boucle colonne de Z
  for(int i=0;i<=size-1;i++)
  { 
  //colonne de Z sur laquelle on va travailler (variable a gauche)
    Eigen::MatrixXd colZ=Z.block(0,i,size,1);
    //si la somme de la colonne=0 alors on appelle mixmode
    if(colZ.sum()==0)
    {
      BicRes(i)=Bic_vide_vect(i,0);
    }
    else
    {
    //calcul du nouveau BIC    
      int newsize=colZ.sum();
      mat_var_a_droite.resize(sizerow,newsize);
      int k=0;
    //on echerche l'emplacement des 1 dans le vecteur colonne (variables a droite)
      for(int j=0;j<size;j++)
      {
        if (colZ(j,0)==1)
        {
        //on regroupe les variables a droite dans une matrice
          mat_var_a_droite.block(0,k,sizerow,1)=X.block(0,j,sizerow,1);
          k=k+1;
        }//fin if
      }//fin for j
    
      //calcul de son BIC en appellant OLS (les parametres sont : Y=X[,colonne],X=X[,matrice des variables à droite])
      BicRes(i)=BicLoc_cpp(mat_var_a_droite,X.block(0,i,sizerow,1),"T",methode);
    }//fin else
    //stockage du BIC dans le vecteur
  }//fin for i
  return BicRes;
}



//colZ en vecteur
//parametre sum=F par defaut
