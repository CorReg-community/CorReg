#include "kronecker_prod_cpp.h"
#include "kronecker_sum_cpp.h"
#include <iostream>
#include <stdlib.h>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

extern "C" SEXP GM_likelihood(SEXP RY,SEXP RX,SEXP RBeta,SEXP RSigma,SEXP RM,SEXP Rnbclustmixmod,SEXP Rdetailsmixmod,SEXP RLog,SEXP Rintercept)
{
  BEGIN_RCPP
//  Rcpp::NumericMatrix matX(RX),beta(RBeta);
//  const Map<MatrixXd> matX(as<Map<MatrixXd> >(RX));//vecteur ligne des variables a droite (completee pour ne pas avoir de trous)
  const Map<VectorXd> matM(as<Map<VectorXd> >(RM));//vecteur ligne indicatrice des manquants (format eigen pour fonction sum)
//  const Map<MatrixXd> beta(as<Map<MatrixXd> >(RBeta));//Beta obtenu par OLS (format matriciel)
//  const Map<VectorXd> Y(as<Map<VectorXd> >(RY));//scalaire valeur de la variable a gauche
  Rcpp::NumericVector Y(RY),matX(RX),beta(RBeta),nbclustmixmod(Rnbclustmixmod);// vecteurs Rcpp
  Rcpp::NumericMatrix detailsmixmod(Rdetailsmixmod);
  double sigma = Rcpp::as<double>(RSigma);//ecart-type (estime) du residu (on l'adaptera eventuellement si trous mais une seule classe)
  const bool loglik = Rcpp::as<bool>(RLog);     // 
  const bool intercept = Rcpp::as<bool>(Rintercept);// 
  int j=0;//on le met ici pour eviter un warning
  const int p=matX.size();
  RNGScope scope;

  //
  double meanfix=0;

  NumericVector res(1);
  //calcul de la loi normale hors manquants
  int inter=0;
  if(intercept){
    inter=1;
    meanfix=beta(0);
    for(int i=0;i<p;i++){
      meanfix=meanfix+(1-matM(i))*matX(i)*beta(i+1);
    }
  }else{
    for(int i=0;i<p;i++){
      meanfix=meanfix+(1-matM(i))*matX(i)*beta(i);
    }
  }
  
  double nbmank=matM.sum();//nombre de manquants
  if(nbmank>0){
    double nbclass=1;
    NumericVector quimank(nbmank);
    NumericVector nbclustcumsum(nbclustmixmod.size());
    //on calcule la somme cumulee
    std::partial_sum(nbclustmixmod.begin(), nbclustmixmod.end(), nbclustcumsum.begin());
    j=0;
    for(int i=0;i<p;i++){
      if(matM(i)==1){//si la valeur est manquante, on multiplie les composantes (kronecker)
        nbclass=nbclass*nbclustmixmod(i);
        quimank(j)=i;//attention, on a deja un indice qui commence en 0
        j++;
      }
    }
    //on sait maintenant precisement qui manque et ou trouver les parametres associes
    if(nbclass==1){// la loi est normale quand-meme
      sigma=sigma*sigma;//on passe au carre car on va ajouter les variances
      for(int i=0;i<nbmank;i++){//pour chaque manquant
        meanfix=meanfix+detailsmixmod(nbclustcumsum(quimank(i))-1,1)*beta(quimank(i)+inter);//colonne 1 est la deuxieme colonne donc la moyenne
        sigma=sigma+detailsmixmod(nbclustcumsum(quimank(i))-1,2)*beta(quimank(i)+inter)*beta(quimank(i)+inter);
      }
      res=Rcpp::wrap(dnorm(Y,meanfix,sqrt(sigma)));//on a des variances donc on passe a la racine
    }else{//plusieurs classes
      NumericVector proptot(1);
      NumericVector vartot(1);
      NumericVector meantot(1);//en deux temps pour qu'il cree bien des vecteurs
      meantot(0)=meanfix;
      proptot(0)=1;
      vartot(0)=sigma*sigma;      
      for(int i=0;i<nbmank;i++){//pour chaque manquant
        //on cree les vecteurs des parametres
        NumericVector proploc(nbclustmixmod(quimank(i)));
        NumericVector meanloc(nbclustmixmod(quimank(i)));
        NumericVector varloc(nbclustmixmod(quimank(i)));
        for(int j=0;j<nbclustmixmod(quimank(i));j++){
          proploc(j)=detailsmixmod(nbclustcumsum(quimank(i))-nbclustmixmod(quimank(i))+j,0);
          meanloc(j)=detailsmixmod(nbclustcumsum(quimank(i))-nbclustmixmod(quimank(i))+j,1)*beta(quimank(i)+inter);
          varloc(j)=detailsmixmod(nbclustcumsum(quimank(i))-nbclustmixmod(quimank(i))+j,2)*beta(quimank(i)+inter)*beta(quimank(i)+inter);
        }
        proptot=kronecker_prod_cpp(proptot,proploc);
        meantot=kronecker_sum_cpp(meantot,meanloc);  
        vartot=kronecker_sum_cpp(vartot,varloc);
      }
      double resloc;
      for(int i=0;i<nbclass;i++){
        resloc=Rcpp::as<double>(Rcpp::wrap(dnorm(Y,meantot(i),sqrt(vartot(i)))));
        res=res+proptot(i)*resloc;//on a des variances donc on passe a la racine
      }
    }//fin du if plusieurs classes
  }else{//il ne manque rien donc loi normale
    res=Rcpp::wrap(dnorm(Y,meanfix,sigma));
  }
  
///      proptot=kronecker(proptot,mixmod$details[[i]][,1])
//      meantot=kroneckersum_vect(meantot,mixmod$details[[i]][,2]*B[-1][i])
//      vartot=kroneckersum_vect(vartot,mixmod$details[[i]][,3]*(B[-1][i])^2)    
//    }
//    #calcul de la vraisemblance
//    for(i in 1:length(proptot)){
//      res=res+proptot[i]*dnorm(Y,mean=meantot[i],sd=sqrt(vartot[i])) 
  if(loglik){
    res=log(res);
  }  
  return res;
  END_RCPP
}
