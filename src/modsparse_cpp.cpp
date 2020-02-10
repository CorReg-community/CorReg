#include "modsparse_cpp.h"
#include <iostream>
#include <string>
#include <math.h>
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP modsparse_cpp(SEXP Zi,SEXP Zj,SEXP Si,SEXP Sj,SEXP complexite,SEXP p,SEXP i,SEXP j,SEXP relax,SEXP rmax)
{
  BEGIN_RCPP
  //declaration des variables
  Map<VectorXd> zi(as<Map<VectorXd> >(Zi));//bic_vide_vect
  Map<VectorXd> zj(as<Map<VectorXd> >(Zj));//bic_vide_vect
  Map<VectorXd> sj(as<Map<VectorXd> >(Sj));//bic_vide_vect
  Map<VectorXd> si(as<Map<VectorXd> >(Si));//bic_vide_vect

  Rcpp::NumericVector I(i),J(j),Relax(relax),Rmax(rmax),Complexite(complexite);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator iI = I.begin(),iJ=J.begin(),irelax=Relax.begin(),irmax=Rmax.begin(),icomplexite=Complexite.begin();
  
  int value=-1;
  bool mod=true;
  int debut_colj=0;
  int compte;
  Eigen::VectorXd zj_loc;
  Eigen::VectorXd zi_loc;
  bool ajout=false;
  int k;
  
  if(iJ[0]>1)//si on est pas en premiere colonne
  {
    for(k=0;k<iJ[0]-1;k++)//on regarde ou commence la colonne j
    {
      debut_colj=debut_colj+sj(k);
    }
  }
  compte=debut_colj;
  
  if(si(iI[0]-1)!=0 && sj(iJ[0]-1)!=0)
  {
    while(compte<(debut_colj+sj(iJ[0]-1)) )
    {
      if(zi(compte)==iI[0])
      {
        value=compte;
        compte=icomplexite[0]+10;//on sort de la boucle
      }
      else if (zi(compte)>iI[0])
      {
        compte=icomplexite[0]+10;//on sort de la boucle
      }
      else//on continue de chercher
      {
        compte=compte+1;
      }
    }
  }
  if(value==-1)//mode ajout
  {
    if(sj(iJ[0]-1)<irmax[0])//pas de surcomplexite
    {
      if(si(iJ[0]-1)!=0 && sj(iI[0]-1)!=0)//pas de croisement
      {
        for(k=0;k<icomplexite[0];k++)
        {
          if(ajout==false && (zj(k)>iJ[0] ||(zj(k)==iJ[0] && zi(k)>iI[0]) ))//on ajoute
          {            
            zj_loc<<zj_loc,iJ[0];//on passe par zjloc
            zi_loc<<zi_loc,iI[0];
            ajout=true;
          }
          zj_loc<<zj_loc,zj(k);
          zi_loc<<zi_loc,zi(k);
        }
        zi=zi_loc;
        zj=zj_loc;
        si(iI[0]-1)=si(iI[0]-1)+1;
        sj(iJ[0]-1)=sj(iJ[0]-1)+1;
        icomplexite[0]=icomplexite[0]+1;
        if(ajout==false)//derniere position
        {
          zj<<zj,iJ[0];
          zi<<zi,iI[0];
        }
      }
      else //croisement
      {
        if(irelax[0]==1)//on fait la relaxation
        {
          
          for(k=0;k<icomplexite[0];k++)
          {
            if(ajout==false && (zj(k)>iJ[0] ||(zj(k)==iJ[0] && zi(k)>iI[0]) ))//on ajoute
            {            
              zj_loc<<zj_loc,iJ[0];//on passe par zjloc
              zi_loc<<zi_loc,iI[0];
              ajout=true;
            }
            if(zj(k)!=iI[0]&&zi(k)!=iJ[0])//si on doit pas supprimer, on recopie
            {
              zj_loc<<zj_loc,zj(k);
              zi_loc<<zi_loc,zi(k);
            }
            else
            {
              sj(zj(k)-1)=sj(zj(k)-1)-1;
              si(zi(k)-1)=si(zi(k)-1)-1;
              icomplexite[0]=icomplexite[0]-1;
            }
          }
          zi=zi_loc;
          zj=zj_loc;
          si(iI[0]-1)=si(iI[0]-1)+1;
          sj(iJ[0]-1)=sj(iJ[0]-1)+1;
          icomplexite[0]=icomplexite[0]+1;
          if(ajout==false)//derniere position
          {
            zj<<zj,iJ[0];
            zi<<zi,iI[0];
          }
        }
        else //mode rejet
        {
          mod=false;
        }
      }
    }
    else //surcomplexite
    {
      mod=false;
    }
  }
  else//suppression
  {
    for(k=0;k<icomplexite[0];k++)
    {
      if(k!=value)
      {
        zj_loc(k)=zj(k);
        zi_loc(k)=zi(k);
      }
    }
    zi=zi_loc;
    zj=zj_loc;
    si(iI[0]-1)=si(iI[0]-1)-1;
    sj(iJ[0]-1)=sj(iJ[0]-1)-1;
    icomplexite[0]=icomplexite[0]-1;
  }
  return List::create(
      Named("Zi")=  zi,
      Named("Zj")=  zj,
      Named("Si")=  si,
      Named("Sj")=  sj,
      Named("complexite")=  complexite,
      Named("mod")=  mod
    );
  
  END_RCPP
}
