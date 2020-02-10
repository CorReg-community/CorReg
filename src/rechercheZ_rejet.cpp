#include "rechercheZ_rejet.h"
#include "BicZ_cpp.h"
#include "BicZ_j_cpp.h"
#include "ProbaZ_cpp.h"
#include <iostream>
#include <string>
#include <math.h>
//#include <Rmath.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP rechercheZ_rejet(SEXP X,SEXP Z,SEXP bic_vide_vect,SEXP methode_tirage,SEXP methode_BIC,SEXP Rmax,SEXP p2max,SEXP Maxiter,SEXP plot,SEXP best,SEXP better,SEXP random,SEXP bla,SEXP nb_opt_max,SEXP Rexact,SEXP Rstar)
{
 
  BEGIN_RCPP
  //déclaration des varibles
  const Map<MatrixXd> matZ(as<Map<MatrixXd> >(Z));//Z
  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<VectorXd> Bic_vide_vect(as<Map<VectorXd> >(bic_vide_vect));//bic_vide_vect
  bool Exact = Rcpp::as<bool>(Rexact);     // length vector 
  bool star = Rcpp::as<bool>(Rstar);     // BICstar

  Rcpp::NumericVector met_tirage(methode_tirage),met_BIC(methode_BIC),rmax(Rmax),P2max(p2max),maxiter(Maxiter),Plot(plot),Best(best),Better(better),Random(random),Bla(bla),Nb_opt_max(nb_opt_max);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imet_tirage = met_tirage.begin(),imet_BIC=met_BIC.begin(),irmax=rmax.begin(),ip2max=P2max.begin(),imaxiter=maxiter.begin(),iplot=Plot.begin(),ibest=Best.begin(),ibetter=Better.begin(),irandom=Random.begin(),ibla=Bla.begin(),inb_opt_max=Nb_opt_max.begin();
  int p=matZ.cols();//nombre de colonne de la matrice Z
  Eigen::MatrixXd Zopt;//meilleur Z obtenu
  double Bicbest;//BIC associé au meilleur modèle
  Eigen::VectorXd bicvect;//vecteur BIC des matrices Z retenues
  Eigen::MatrixXd newZ;//matrice Z modifie a chaque etapes
  Eigen::MatrixXd list_cand;//matrice qui contient les coordonnées des candidats
  int nbcand=0;//nombre de candidats
  int nb_opt;//nombre de fois ou on a retrouve bicbest
  int step_opt=0;//etape ou l'on decouvre BIC_opt
  int sumZ=0;
  int k;//nombre du tirage aleatoire (liste candidats)
  int compte;//permet de cree la matrice liste en désignant le numéro du candidat (liste candidats)
  int i;//coordonnée (x ou y) des candidats (liste candidats)
  int rand1;//nombre aleatoire pour la 3ème méthode de tirage des candidats (liste candidats)
  int rand2;//nombre aleatoire pour la 3ème méthode de tirage des candidats (liste candidats)
  int numcand;//numero du candidat
  int i_loc;//premiere coordonnée du candidat (modification Z)
  int j_loc;//duexième coordonnée du candidat (modification Z)
  int realisable;//booléen qui permet de savoir si on peut effectuer un changement dans Z (modification Z)
  Eigen::MatrixXd Zcand;//matrice Z du candidat (modification Z)
  int u;// (modification Z)
  Eigen::ArrayXXd SumCol(1,Zcand.cols());//k est un vecteur qui contient la somme de chaque colonne de Zcand (modification Z)
//  int val;//permet de mettre une colonne et une ligne a 0 (modification Z)
  Eigen::VectorXd BIC_cand;//vecteur qui contient les BIC de chaque colonne des la matrice Zcand (calcul du BIC)
  double Sum_BIC_cand;//somme des BIC de BIC_cand (calcul BIC)
  Eigen::VectorXd stock_bool;//vecteur qui permet de savoir si un candidat peut etre choisi (stock)
  Eigen::VectorXd stock_BIC;//vecteur qui contient le BIC de tout les candidats (stock)
  double sumbic;//BIC de chaque etapes
  
  Eigen::VectorXd bic_etape ( static_cast<int>(imaxiter[0]) );//vecteur qui stock le BIC de chaque etapes
  Eigen::VectorXd complexite_etape ( static_cast<int>(imaxiter[0]) );//vecteur qui stock la compléxité de Z a chaque etapes
  Eigen::VectorXd etape ( static_cast<int>(imaxiter[0]) );//vecteur qui stock le type de changement de chaque etapes
  bool station;//permet de savoir si on est stationnaire ou non

    
  //initialisation
  Zopt = MatrixXd::Zero(p,p);
//somme a la main
  Bicbest = Bic_vide_vect.sum();
  bicvect=BicZ_cpp(matX,matZ,Bic_vide_vect,Bic_vide_vect,imet_BIC[0],Zopt);
//somme a la main
  sumbic=bicvect.sum();
   if(star){
      sumbic=sumbic-ProbaZ_cpp(matZ);
   }
  if (ibla[0]>0)
  {
    Rcout<<sumbic<<"\n";
  }
  if (sumbic<Bicbest)//si Z est meilleur que la matrice nulle alors on met a jour bicbest et Zopt
  {
    Bicbest =sumbic;
    Zopt=matZ;
  }
  newZ=matZ;//pour la premiere etape, newZ est la matrice de depart
  
  int step =0;
  
  if (imet_tirage[0]==0)//methode de changement de la ligne et de la colonne
  {
    nbcand=2*p-2;
    list_cand.resize(nbcand,2);//2p-2 candidats
  }
  else if(imet_tirage[0]==-1)
  {
    nbcand=p-1;
    list_cand.resize(nbcand,2);//p-1 candidats (colonne uniquement)
  }
  else if(imet_tirage[0]==-2)
  {
    nbcand=p*(p-1);
    list_cand.resize(nbcand,2);//p-1 candidats (colonne uniquement)
  }
  else if(imet_tirage[0]>0)//methode de tirage aléatoire
  {
    nbcand=imet_tirage[0];
    list_cand.resize(nbcand,2);//le nombre de candidats est determine pas l'utilisateur
  }
  else if(imet_tirage[0]!=-3) //si methode a une valeur aberante
  {
    throw std::range_error("methode de tirage incorrecte");
  }
  
  if (irandom[0]==0)
  {
    ibetter[0]=1;
    ibest[0]=1;
  }
  else if (ibetter[0]==1)
  {
    ibest[0]=1;
  }
  
  nb_opt=0;
    
  
  while(step<imaxiter[0])
  // initialisation des variables
  
  {
    compte=0;//initialisation du vecteur liste désigne le numéro du candidat 
  //liste candidats (couples [i,j])
    if (imet_tirage[0]==0)//methode de changement de la ligne et de la colonne
    {
      k=int(p*runif(1)[0]);//on tire un numero aleatoire pour savoir quel est la ligne et la colonne de candidats
      for(i=0;i<p;i++)
      {
        if(i!=k)//on ne veut pas avoir de 1 sur la diagonale
        {
          list_cand(compte,0)=i;
          list_cand(compte,1)=k;
          compte=compte+1;
          list_cand(compte,0)=k;
          list_cand(compte,1)=i;
          compte=compte+1;
        }
      }    
    }
    else if (imet_tirage[0]==-1)//seulement la colonne
    {
      k=int(p*runif(1)[0]);
      for(i=0;i<p;i++)
      {
        if(i!=k)
          {
            list_cand(compte,0)=i;
            list_cand(compte,1)=k;
            compte=compte+1;
          }
      }
    }
    else if (imet_tirage[0]==-2)//seulement la colonne
    {
      for(k=0;k<p;k++)
      {
        for(i=0;i<p;i++)
        {
          if(i!=k)
            {
              list_cand(compte,0)=i;
              list_cand(compte,1)=k;
              compte=compte+1;
            }
        }
      }
    }
    else if (imet_tirage[0]==-3)//tout le monde sauf la diagonale
    {
      nbcand=0;
      sumZ=newZ.sum();
      if(sumZ!=0)
      {
        list_cand.resize(newZ.sum(),2);//autant de candidats que de points
      }
      for(k=0;k<p;k++)
      {
        for(i=0;i<p;i++)
        {
          if(i!=k)
          {
            if(newZ(i,k)!=0)
            {
              list_cand(compte,0)=i;
              list_cand(compte,1)=k;
              compte=compte+1;
              nbcand++;
            }
          }
        }
      }
      if(nbcand==0)//aucun candidat donc on arrete
      {
        step=imaxiter[0]+10;
      }
    }
    else if (imet_tirage[0]>0)//methode de tirage aléatoire
    {
      for(i=0;i<nbcand;i++)
      {
        rand1=int(p*runif(1)[0]);//nombres aleatoire pour avoir le numero de la ligne
        rand2=int(p*runif(1)[0]);//nombres aleatoire pour avoir le numero de la colonne
        if (rand1==rand2)//on ne veut pas de 1 sur la diagonale
        {
          if(rand1<p-1)
          {
            rand2=rand1+1;
          }
          else
          {
            rand2=rand1-1;
          }
        }
        list_cand(i,0)=rand1;
        list_cand(i,1)=rand2;            
      }
    }
  
    
    //pour chaque candidat
    for (numcand=0;numcand<nbcand;numcand++)
    {
    //modification (calcul du Z)    (Zcand avec methode (rejet ou relax)   Z,i,j,methode="relax", p2max=inf,rmax=inf)
      i_loc=list_cand(numcand,0);
      j_loc=list_cand(numcand,1);
      realisable=1;//le changement est vrai par defaut
      Zcand=newZ;
      Zcand(i_loc,j_loc)=1-Zcand(i_loc,j_loc);
      if (Zcand(i_loc,j_loc)==1)//probleme potentiel
      {
//        if (imet_newZ[0]==1)//mode rejet
//        {
          u=0;
          while(realisable==1 && u<p)
          {
            if (Zcand(u,i_loc)!=0 ||Zcand(j_loc,u)!=0)//regarde si la modif peut etre effectue
            {
              realisable=0;//il y'a un croisement
            }
            u++;
          }
          
          if (realisable ==1)
          {
            if (Zcand.col(j_loc).sum()>irmax[0])
            {
              realisable=0;
            }
            else
            {
              SumCol=Zcand.colwise().sum();
              if ((SumCol > 0).count()>ip2max[0])//si k a plus de p2max valeur > 0 alors on ne modifiera pas la matrice
              {
                realisable=0;
              }
            }
          }

      }
    //calcul du bic (du nouveau Z généré)      (bicZ)
      if (realisable==0)
      {
        Sum_BIC_cand=0;
      }
      else
      {
        BIC_cand=BicZ_j_cpp(matX,Zcand,Bic_vide_vect,bicvect,imet_BIC[0],j_loc);
        if(Exact){
          if(BIC_cand(j_loc)==0){
             Rcout <<j_loc+1<< " depends on";
              //on recherche l'emplacement des 1 dans le vecteur colonne (variables a droite)
              for(int i=0;i<=p-1;i++)
              {
                if (Zcand(i,j_loc)==1)
                {
                 Rcout<< " "<< i+1 << " ";
                }//fin if
              }//fin for i
              Rcout<<"\n";
              step=imaxiter[0];
          }
        }
        
        Sum_BIC_cand=BIC_cand.sum();
        if(star){
           Sum_BIC_cand=Sum_BIC_cand-ProbaZ_cpp(Zcand);
        }
      }
    //stockage des valeurs (bic,changement)
      stock_bool.resize(nbcand);
      stock_bool(numcand)=realisable;
      stock_BIC.resize(nbcand);
      stock_BIC(numcand)=Sum_BIC_cand;
    }
    //choix du candidat retenu (tirage au sort ou meilleur selon le cas)

    int nb_cand_retenu=stock_bool.sum()+1;//nombre de candidats retenu (certain ne peuvent pas etre pri en compte)
// à faire à la main

    Eigen::VectorXd ind_stock_bool1(nb_cand_retenu-1);//vecteur contenant les numeros des individus retenus
    Eigen::VectorXd stockBIC1(nb_cand_retenu);//vecteur contenant les bic des individus retenus
    int compteur=0;//compteur permattant de remplir stockBIC1 et ind_stock_bool1
    int bettercand=0;
    int iret,jret;//coordonnees du candidat ayant le meilleur bic depuis le debut
    double sumcum=0;
//    int cha;
    int w;
    NumericVector ran;
    Eigen::VectorXd bicweight(nb_cand_retenu);
    double sumexp=0;
    double BIC_min=sumbic;//valeur pour le Z actuel
    station=false;
    stockBIC1(0)=sumbic;
    for (i=0;i<stock_BIC.size();i++)//on garde que les individus retenus
    {
      if(stock_bool(i,0)==1)
      {
        ind_stock_bool1(compteur)=i;//seulement les individus possibles
        stockBIC1(compteur+1)=stock_BIC(i);
        if(stockBIC1(compteur+1)<BIC_min)//on cherche le plus petit BIC
        {
          BIC_min=stockBIC1(compteur+1);
          bettercand=compteur+1;
        }
        compteur=compteur+1;
      }     
    } 
    
    //prise de décision
    if(BIC_min<Bicbest) //regarde s'il y'a un BIC meilleur que le meuilleur rencontré
    {
      step_opt=step;
      nb_opt=0;//il y a un nouveau bicbest donc le compteur revient a 0
      Bicbest=BIC_min;
      iret=list_cand(ind_stock_bool1(bettercand-1),0);//-1 a cause de la stationarité
      jret=list_cand(ind_stock_bool1(bettercand-1),1);
      Zopt=newZ;//initialisation à la stationarité
      Zopt(iret,jret)=1-Zopt(iret,jret);
      
      if (ibla[0]>0)
      {
        Rcout<<step<<" Bicbest: "<<Bicbest<<" complexite: "<<Zopt.sum()<<"\n";
        if (ibla[0]==2)
        {
          Rcout<<"nb_cand "<<nb_cand_retenu<<"BIC min "<<BIC_min<<"\n";
        }
      }
     
      
      if(ibest[0]==1)//on a vu le meilleur absolu donc si on veut y aller, on y va
      { 
        bicvect=BicZ_j_cpp(matX,Zopt,Bic_vide_vect,bicvect,imet_BIC[0],jret);
        newZ=Zopt;
        sumbic=Bicbest;
      }
      else //tirage au sort
      {
        bicweight=stockBIC1;
        for (int v=0;v<nb_cand_retenu;v++)
        {
          bicweight(v)=bicweight(v)-BIC_min;
          bicweight(v)=exp(-bicweight(v)/2);
          sumexp=sumexp+bicweight(v);
        }
        w=-1;//initialisation candidat choisi (il est a -1 car on va rajouter un "+1" avec le while)
        ran=runif(1);
        sumcum=0;
        while (ran[0]>=sumcum)//choix du candidat
        { 
          w++;
          bicweight(w)=bicweight(w)/sumexp;
          sumcum=sumcum+bicweight(w);
        }
        if(w==0)
        {
         station=true;
        }
        else
        {
          iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
          jret=list_cand(ind_stock_bool1(w-1),1);
          Zcand=newZ;
          Zcand(iret,jret)=1-Zcand(iret,jret);

          bicvect=BicZ_j_cpp(matX,Zcand,Bic_vide_vect,bicvect,imet_BIC[0],jret);
          newZ=Zcand;
          sumbic=stockBIC1(w);
        }
      }
    } 
    else if(nb_cand_retenu==1) //si la stationarité est le seul candidat
    {
      station=true;
    }
    else if(ibetter[0]==1 && bettercand!=0)//si on a un meilleur local et qu'on veut y aller, on y va
    {
      iret=list_cand(ind_stock_bool1(bettercand-1),0);//-1 a cause de la stationarité
      jret=list_cand(ind_stock_bool1(bettercand-1),1);
      Zcand=newZ;
      Zcand(iret,jret)=1-Zcand(iret,jret);

      bicvect=BicZ_j_cpp(matX,Zcand,Bic_vide_vect,bicvect,imet_BIC[0],jret);
      newZ=Zcand;
      sumbic=BIC_min;
    }
    else if (irandom[0]==0)
    {
      station=true;
    }
    else//tirage au sort (pas de saut systématique)
    {
      bicweight=stockBIC1;
      for (int v=0;v<nb_cand_retenu;v++)
      {
        bicweight(v)=bicweight(v)-BIC_min;
        bicweight(v)=exp(-bicweight(v)/2);
        sumexp=sumexp+bicweight(v);
      }
      
      w=-1;//initialisation candidat choisi (il est a -1 car on va rajouter un "+1" avec le while)
      ran=runif(1);
      sumcum=0;
      while (ran[0]>=sumcum)//choix du candidat
      { 
        w++;
        bicweight(w)=bicweight(w)/sumexp;
        sumcum=sumcum+bicweight(w);
      }
      if(w==0)
      {
       station=true;
      }
      else
      {
        iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
        jret=list_cand(ind_stock_bool1(w-1),1);
        Zcand=newZ;
        Zcand(iret,jret)=1-Zcand(iret,jret);

        bicvect=BicZ_j_cpp(matX,Zcand,Bic_vide_vect,bicvect,imet_BIC[0],jret);
        newZ=Zcand;
        sumbic=stockBIC1(w);
      }
    }
    
    if(sumbic==Bicbest)//on regarde si le le BIC est le meme que le BIC optimal
    {
      nb_opt=nb_opt+1;//si c'est le cas alors on a retrouve une fois de plus BICbest
      if(nb_opt==inb_opt_max[0])//si on a atteint le nombre maximum de fois ou on a retrouvé BICbest
      {
        imaxiter[0]=step;//alors on sort du while
        Rcout<<"convergence atteinte";
      }
    }
    
    
    if(iplot[0]==1)//si on veut des graphiques on mets à jour les vecteurs
    {
      bic_etape (step)=sumbic;
      complexite_etape (step)=newZ.sum();

      if (station==true)
      {
        etape (step)=2;//stationarité
      }
      else if(newZ(iret,jret)==0)
      {
        etape (step)=0;//suppression
      }
      else
      {
        etape (step)=1;//ajout
      }
    }
    if(ibla[0]>2)
    {
      Rcout<<step<<" BIC local: "<<sumbic<<" local complexity: "<<newZ.sum()<<"\n";
    }
    step=step+1; 
  }//fin des étapes (while)

  if(iplot[0]==1)
  {
    Eigen::VectorXd BIC_step(step);//on met les vecteurs sorti de bonne dimension
    Eigen::VectorXd complexite_step(step);
    if(step>1)//s'il y'a eu au moins une etape alors on tranfert les donnees des vecteurs
    {
      for(int k=0;k<step;k++)
      {
        BIC_step(k)=bic_etape(k);
        complexite_step(k)=complexite_etape(k);
      }
    }
    
    return List::create(
      Named("Z")=  newZ,
      Named("bic_loc")=  sumbic,
      Named("Z_opt")=  Zopt,
      Named("bic_opt")=  Bicbest,
      Named("bic_step")=  BIC_step,
      Named("complexity_step")=  complexite_step,
      Named("step")=  etape,
      Named("step_opt")=  step_opt
    );
  }
  else
  {
  return List::create(
      Named("Z")=  newZ,
      Named("bic_loc")=  sumbic,
      Named("Z_opt")=  Zopt,
      Named("bic_opt")=  Bicbest,
      Named("step_opt")=  step_opt
    );
  }
  END_RCPP
}
