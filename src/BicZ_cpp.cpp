#include "BicZ_cpp.h"
#include "BicLoc_cpp.h"
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace Rcpp ;
using namespace std;
using namespace Eigen;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

Eigen::VectorXd BicZ_cpp(Eigen::MatrixXd X,Eigen::MatrixXd Z,Eigen::VectorXd Bic_vide_vect,Eigen::VectorXd BicOld,double methode,Eigen::MatrixXd Zold){

int size = X.cols();
int sizerow = X.rows();
Eigen::VectorXd BicRes(size);
Eigen::MatrixXd mat_var_a_droite;
double BicCol;
Eigen::MatrixXd val;
//debut de la boucle colonne de Z
  for(int i=0;i<=size-1;i++){ 
  
  //colonne de Z sur laquelle on va travailler (variable a gauche)
    Eigen::MatrixXd colZ=Z.block(0,i,size,1);
    //si la somme de la colonne=0 alors on appelle mixmod
    if(colZ.sum()==0){
      
    BicCol=Bic_vide_vect(i,0);
    }else{
    //si la colonne est la meme que la precedente matrice Z alors on recupere son BIC
      if(colZ==Zold.block(0,i,size,1))
      {
        BicCol=BicOld(i,0);
      }else{
    //calcul du nouveau BIC    
      int newsize=colZ.sum();
      mat_var_a_droite.resize(sizerow,newsize);
      int k=0;
    //on recherche l'emplacement des 1 dans le vecteur colonne (variables a droite)
    for(int j=0;j<=size-1;j++){
      if (colZ(j,0)==1){
      //on regroupe les variables a droite dans une matrice
      mat_var_a_droite.block(0,k,sizerow,1)=X.block(0,j,sizerow,1);
      k=k+1;
      }//fin if
    }//fin for j
  
    //calcul de son BIC en appellant OLS (les parametres sont : Y=X[,colonne],X=X[,matrice des variables à droite])
   BicCol=BicLoc_cpp(mat_var_a_droite,X.block(0,i,sizerow,1),"T",methode);
   if(BicCol==0){//a priori on a un modele exact (peu probable que le BIC soit vraiment nul)
        Rcpp::Rcout<<i+1<< " depends on";
        //on recherche l'emplacement des 1 dans le vecteur colonne (variables a droite)
        for(int j=0;j<=size-1;j++)
        {
          if (colZ(j,0)==1)
          {
           Rcpp::Rcout<< " "<< j +1<< " ";
          }//fin if
        }//fin for j
  }
   val=X.block(0,i,sizerow,1);
    }//fin else
    }//fin else
    //stockage du BIC dans le vecteur
    BicRes(i)=BicCol;
  }//fin for i
  return BicRes;
    
}



//colZ en vecteur
//parametre sum=F par defaut
