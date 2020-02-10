#include "cleancolZ.h"
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

SEXP cleancolZ(SEXP X,SEXP Z,SEXP bic_vide_vect,SEXP methode_BIC,SEXP plot,SEXP bla,SEXP Rstar)
{
  BEGIN_RCPP
  //declaration des variables
  const Map<MatrixXd> matZ(as<Map<MatrixXd> >(Z));//Z
  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<VectorXd> Bic_vide_vect(as<Map<VectorXd> >(bic_vide_vect));//bic_vide_vect
  bool star = Rcpp::as<bool>(Rstar);     // BICstar

  Rcpp::NumericVector met_BIC(methode_BIC),Plot(plot),Bla(bla);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imet_BIC=met_BIC.begin(),iplot=Plot.begin(),ibla=Bla.begin();
  int p=matZ.cols();//nombre de colonne de la matrice Z
  Eigen::MatrixXd Zopt;//meilleur Z obtenu
  double Bicbest;//BIC associe au meilleur modele
  Eigen::VectorXd bicvect;//vecteur BIC des matrices Z retenues
  Eigen::MatrixXd newZ;//matrice Z modifie a chaque etapes
  Eigen::VectorXd list_cand;//vecteur qui contient les indices des colonnes candidates (liste candidats)
  int compte=0;//permet de cree la matrice liste en designant le numero du candidat (liste candidats)
  int nbcand;//nombre de candidats
  int numcand;//numero du candidat
  int i_loc;//premiere coordonnee du candidat (modification Z)
  Eigen::MatrixXd Zcand;//matrice Z du candidat (modification Z)
  Eigen::ArrayXXd SumCol(1,p);//SumCol est un vecteur qui contient la somme de chaque colonne de Zcand (modification Z)
  Eigen::VectorXd BIC_cand;//vecteur qui contient les BIC de chaque colonne des la matrice Zcand (calcul du BIC)
  double Sum_BIC_cand;//somme des BIC de BIC_cand (calcul BIC)
  Eigen::VectorXd stock_BIC;//vecteur qui contient le BIC de tout les candidats (stock)
  double sumbic;//BIC de chaque etapes
  double BIC_min;
  bool minimum;
  int i;
  int iret=1;
  Eigen::VectorXd bic_etape;//vecteur qui stock le BIC de chaque etapes
  Eigen::VectorXd complexite_etape;//vecteur qui stock la complexite de Z a chaque etapes
  Eigen::VectorXd BIC_step;//vecteur qui stock le BIC de chaque etapes
  Eigen::VectorXd complexite_step ;//vecteur qui stock la complexite de Z a chaque etapes
  //initialisation
  bicvect=BicZ_cpp2(matX,matZ,Bic_vide_vect,imet_BIC[0]);
//somme a la main
  sumbic=bicvect.sum();
  if(star)
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
  
  int step =0;
  SumCol=matZ.colwise().sum();//nombre d'elements dans chaque colonne
  nbcand=(SumCol>0).count();//nombre de colonnes candidates (quand il y'a au moins 1 element dans une colonne)
  list_cand.resize(nbcand);//le nombre de candidats est la complexite du modele
  bic_etape.resize(nbcand);
  complexite_etape.resize(nbcand);
  while(step<nbcand)  
  {
    compte=0;//initialisation du vecteur liste designe le numero du candidat
    
  //liste candidats (couples [i,j])
    for(int n=0;n<p;n++)//parcours les colonnes
    {
      if(newZ.col(n).sum()>0)//on cherche les candidats
      {
        list_cand(compte)=n;//stock la colonne du candidat
        compte=compte+1;//passage au candidat suivant
      }
    }
    
    stock_BIC.resize(compte);
    //pour chaque candidat
    for (numcand=0;numcand<compte;numcand++)
    {
    //modification (calcul du Z)    (Zcand avec methode (rejet ou relax)   Z,i,j,methode="relax", p2max=inf,rmax=inf)
      
      Zcand=newZ;
      for(i_loc=0;i_loc<p;i_loc++)//parcours des lignes
      {
        Zcand(i_loc,list_cand(numcand))=0;//modification de Z par rapport a la colonne candidate
      }
      
    //calcul du bic (du nouveau Z genere)
      BIC_cand=BicZ_cpp(matX,Zcand,Bic_vide_vect,bicvect,imet_BIC[0],newZ);//calcul du vecteur BIC du candidat
      Sum_BIC_cand=BIC_cand.sum();
      if (star)
      {
         Sum_BIC_cand=Sum_BIC_cand-ProbaZ_cpp(Zcand);
      }
    //stockage du BIC
      
      stock_BIC(numcand)=Sum_BIC_cand;
    }
    //on regarde la meilleur candidat
    BIC_min=Bicbest;//initialisation BIC_min
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
      for(int i_opt=0;i_opt<p;i_opt++)//on met la colonne a 0
      {
        Zopt(i_opt,list_cand(iret))=0;
      }
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
        bic_etape(step)=Bicbest;
        complexite_etape(step)=newZ.sum();
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
  }//fin des étapes
  
  if(iplot[0]==1)//si on veut des graphiques on mets a jour les vecteurs
  {
    BIC_step.resize(nbcand-compte);//on redimensionne pour les plot
    complexite_step.resize(nbcand-compte);
    if(nbcand-compte>0)//si il y'a eu au moins une etape
    {
      for(int k=0;k<nbcand-compte;k++)
      {
        BIC_step(k)=bic_etape(k);
        complexite_step(k)=complexite_etape(k);
      }
    }
    return List::create(
      Named("Z")=  newZ,
 //     Named("Z_opt")=  Zopt,
      Named("bic_opt")=  Bicbest,
      Named("bic_step")=  BIC_step,
      Named("complexity_step")=  complexite_step
    );
  }
  else
  {
  return List::create(
      Named("Z")=  newZ,
 //     Named("Z_opt")=  Zopt,
      Named("bic_opt")=  Bicbest
    );
  }
  END_RCPP
}  
