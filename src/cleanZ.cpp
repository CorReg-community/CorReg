#include "cleanZ.h"
#include "BicZ_cpp2.h"
#include "BicZ_cpp.h"
#include "ProbaZ_cpp.h"

#include <iostream>
#include <string>
#include <math.h>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP cleanZ(SEXP X,SEXP Z,SEXP bic_vide_vect,SEXP methode_BIC,SEXP plot,SEXP bla,SEXP Rstar)
{
 
  BEGIN_RCPP
  //déclaration des varibles
  const Map<MatrixXd> matZ(as<Map<MatrixXd> >(Z));//Z
  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<VectorXd> Bic_vide_vect(as<Map<VectorXd> >(bic_vide_vect));//bic_vide_vect
  bool star = Rcpp::as<bool>(Rstar);     // BICstar

  Rcpp::NumericVector met_BIC(methode_BIC),Plot(plot),Bla(bla);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imet_BIC=met_BIC.begin(),iplot=Plot.begin(),ibla=Bla.begin();
  int p=matZ.cols();//nombre de colonne de la matrice Z
  Eigen::MatrixXd Zopt;//meilleur Z obtenu
  double Bicbest;//BIC associe au meilleur modèle
  Eigen::VectorXd bicvect;//vecteur BIC des matrices Z retenues
  Eigen::MatrixXd newZ;//matrice Z modifie a chaque etapes
  Eigen::MatrixXd list_cand;//matrice qui contient les coordonnées des candidats (liste candidats)
  int compte;//permet de cree la matrice liste en désignant le numéro du candidat (liste candidats)
  int nbcand;//nombre de candidats
  int numcand;//numero du candidat
  int i_loc;//premiere coordonnee du candidat (modification Z)
  int j_loc;//duexieme coordonnee du candidat (modification Z)
  Eigen::MatrixXd Zcand;//matrice Z du candidat (modification Z)
  Eigen::VectorXd BIC_cand;//vecteur qui contient les BIC de chaque colonne des la matrice Zcand (calcul du BIC)
  double Sum_BIC_cand;//somme des BIC de BIC_cand (calcul BIC)
  Eigen::VectorXd stock_bool;//vecteur qui permet de savoir si un candidat peut etre choisi (stock)
  Eigen::VectorXd stock_BIC;//vecteur qui contient le BIC de tout les candidats (stock)
  double sumbic;//BIC de chaque etapes
  double BIC_min;
  bool minimum;
  int i;
  int iret=0;
  nbcand=matZ.sum();
  Eigen::VectorXd bic_etape(nbcand);//vecteur qui stock le BIC de chaque etapes
  Eigen::VectorXd complexite_etape(nbcand);//vecteur qui stock la complexite de Z a chaque etapes
  Eigen::VectorXd BIC_step;//vecteur qui stock le BIC de chaque etapes
  Eigen::VectorXd complexite_step ;//vecteur qui stock la compléxité de Z a chaque etapes
//  Eigen::ArrayXXd SumCol(1,p);//SumCol est un vecteur qui contient la somme de chaque colonne de Zcand (modification Z)

    
  //initialisation
  bicvect=BicZ_cpp2(matX,matZ,Bic_vide_vect,imet_BIC[0]);
//somme a la main
  sumbic=bicvect.sum();
  if (star)
  {
      sumbic=sumbic-ProbaZ_cpp(matZ);
  }
  if (ibla[0]>0)
  {
    Rcout<<sumbic<<"\n";
  }
  Bicbest =sumbic;
  Zopt=matZ;
  newZ=matZ;
  compte=nbcand+1;
  int step =0;
  
  while(step<nbcand)  
  {
    list_cand.resize(compte-1,2);//le nombre de candidats est la complexite du modele
    compte=0;//initialisation du vecteur liste designe le numero du candidat
  //liste candidats (couples [i,j])
//    list_cand.resize(nbcand,2);//le nombre de candidats est la complexite du modele
    
    for(int m=0;m<p;m++)//parcours ligne
    {
      for(int n=0;n<p;n++)//parcours colonne
      {
        if(newZ(m,n)==1)//on cherche les candidats
        {
          list_cand(compte,0)=m;//stock la ligne du candidat
          list_cand(compte,1)=n;//stock la colonne du candidat
          compte=compte+1;//passage au candidat suivant
        }
      }
    }
    //pour chaque candidat
    for (numcand=0;numcand<compte;numcand++)
    {
    //modification (calcul du Z)    (Zcand avec methode (rejet ou relax)   Z,i,j,methode="relax", p2max=inf,rmax=inf)
      i_loc=list_cand(numcand,0);
      j_loc=list_cand(numcand,1);
      Zcand=newZ;
      Zcand(i_loc,j_loc)=1-Zcand(i_loc,j_loc);//modification de Z par rapport au candidat selectionne
      
    //calcul du bic (du nouveau Z genere)      (bicZ)
      BIC_cand=BicZ_cpp(matX,Zcand,Bic_vide_vect,bicvect,imet_BIC[0],newZ);//calcul du vecteur BIC du candidat
      Sum_BIC_cand=BIC_cand.sum();
      if (star)
      {
        Sum_BIC_cand=Sum_BIC_cand-ProbaZ_cpp(Zcand);
      }
    //stockage du BIC
      stock_BIC.resize(compte);
      stock_BIC(numcand)=Sum_BIC_cand;
    }
    //choix du candidat retenu (tirage au sort ou meilleur selon le cas)
    BIC_min=Bicbest;
    minimum=false;
    for(i=0;i<compte;i++)
    {
      if(stock_BIC(i)<Bicbest)//si on a trouve un minimum
      {
        iret=i;//on retient sa position
        minimum=true;
        Bicbest=stock_BIC(i);//on modifie Bicbest
      }
    }
    if (minimum==true)//si il y a un minimum, on y va
    {
      Zopt=newZ;//initialisation a la stationarite
      Zopt(list_cand(iret,0),list_cand(iret,1))=1-Zopt(list_cand(iret,0),list_cand(iret,1));//modification du Zopt
      bicvect=BicZ_cpp(matX,Zopt,Bic_vide_vect,bicvect,imet_BIC[0],newZ);//calcul du vecteur BIC du nouveau Zopt
      newZ=Zopt;
      
      //partie commentaires
      if (ibla[0]>0)
      {
        Rcout<<"etape : "<<step<<"\n";
        Rcout<<" Bicbest: "<<Bicbest<<" complexite: "<<newZ.sum()<<"\n";
        if (ibla[0]==2)
        {
          Rcout<<"nb_cand "<<compte<<"BIC min "<<BIC_min<<"\n";
        }
      }
      
      //partie graphiques
      if(iplot[0]==1)//si on veut des graphiques on mets a jour les vecteurs
      {
        bic_etape (step)=Bicbest;
        complexite_etape (step)=newZ.sum();
      }
      
      
      step=step+1;//passage a l'etape suivante s'il y a eu un minimum
    }
    else//pas de minimum
    {  
      if (ibla[0]>0){
        Rcout<<"nettoyage fini";
      }
      step=nbcand+1;//arret des etapes
    }
  }//fin des etapes
  
  if(iplot[0]==1)
  {
    BIC_step.resize(nbcand-compte);//on met les vecteurs sorti de bonne dimension
    complexite_step.resize(nbcand-compte);
    if(nbcand-compte>0)//s'il y'a eu au moins une etape alors on tranfert les donnees des vecteurs
    {
      for(int k=0;k<nbcand-compte;k++)
      {
        BIC_step(k)=bic_etape(k);
        complexite_step(k)=complexite_etape(k);
      }
    }
    return List::create(
      Named("Z")=  newZ,
      Named("Z_opt")=  Zopt,
      Named("bic_opt")=  Bicbest,
      Named("bic_step")=  BIC_step,
      Named("complexity_step")=  complexite_step
    );
  }
  else
  {
  return List::create(
      Named("Z")=  newZ,
      Named("Z_opt")=  Zopt,
      Named("bic_opt")=  Bicbest
    );
  }
  END_RCPP
}  
