#include "BicZmissRcpp.h"
#include "BicLoc_cpp.h"

#include <iostream>
#include <stdlib.h>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP BicZmissRcpp(SEXP X,SEXP Z,SEXP M,SEXP Bic_vide_vect,SEXP BicOld,SEXP methode,SEXP Zold,SEXP intercept,SEXP mixmod,SEXP nbit,SEXP nbclustmax)
{
  BEGIN_RCPP
  const Map<MatrixXd> matZ(as<Map<MatrixXd> >(Z));//Z
  const Map<MatrixXd> matZold(as<Map<MatrixXd> >(Zold));//Zold
  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<MatrixXd> vide(as<Map<MatrixXd> >(Bic_vide_vect));//Bic_vide_vect
  const Map<MatrixXd> BicO(as<Map<MatrixXd> >(BicOld));//X
  
  Rcpp::NumericVector xmet(methode);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imet = xmet.begin();
  int method =imet[0];
  
  int size = matX.cols();
  int sizerow = matX.rows();
  Eigen::MatrixXd BicRes(size,1);
  Eigen::MatrixXd mat_var_a_droite;
  float BicCol;
  Eigen::MatrixXd val;
  //debut de la boucle colonne de Z
  for(int i=0;i<=size-1;i++)
  { 
  
  //colonne de Z sur laquelle on va travailler (variable a gauche)
    Eigen::MatrixXd colZ=matZ.block(0,i,size,1);
    //si la somme de la colonne=0 alors on appelle mixmod
    if(colZ.sum()==0)
    {
      
      BicCol=vide(i,0);
    }
    else
    {
    //si la colonne est la meme que la precedente matrice Z alors on recupere son BIC
      if(colZ==matZold.block(0,i,size,1))
      {
        BicCol=BicO(i,0);
      }
      else
      {
    //calcul du nouveau BIC    
        int newsize=colZ.sum();
        mat_var_a_droite.resize(sizerow,newsize);
        int k=0;
        //on recherche l'emplacement des 1 dans le vecteur colonne (variables a droite)
        for(int j=0;j<=size-1;j++)
        {
          if (colZ(j,0)==1)
          {
          //on regroupe les variables a droite dans une matrice
            mat_var_a_droite.block(0,k,sizerow,1)=matX.block(0,j,sizerow,1);
            k=k+1;
          }//fin if
        }//fin for j
  
        //calcul de son BIC en appellant OLS (les parametres sont : Y=X[,colonne],X=X[,matrice des variables a  droite])
        BicCol=BicLoc_cpp(mat_var_a_droite,matX.block(0,i,sizerow,1),"T",method);
        if(BicCol==0){//a priori on a un modele exact (peu probable que le BIC soit vraiment nul)
        Rcpp::Rcout <<i+1<< " depends on";
        //on recherche l'emplacement des 1 dans le vecteur colonne (variables a droite)
        for(int j=0;j<=size-1;j++)
        {
          if (colZ(j,0)==1)
          {
           Rcout<< " "<< j+1 << " ";
          }//fin if
        }//fin for j
        }
        val=matX.block(0,i,sizerow,1);
      }//fin else
    }//fin else
    //stockage du BIC dans le vecteur
    BicRes(i,0)=BicCol;
  }//fin for i
  
  return List::create(
      Named("BIC")=  BicRes
    );
  END_RCPP
}
