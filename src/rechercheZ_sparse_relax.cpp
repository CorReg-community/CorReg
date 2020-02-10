 #include "rechercheZ_sparse_relax.h"
 #include "BICZsparse_cpp.h"
 #include "BICZsparse2_cpp.h"
 #include <iostream>
 #include <string>
 #include <math.h>
//#include <Rmath.h>

using namespace Rcpp ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

SEXP rechercheZ_sparse_relax(SEXP X,SEXP Zi,SEXP Zj,SEXP Si,SEXP Sj,SEXP bic_vide_vect,SEXP methode_tirage,SEXP methode_BIC,SEXP Rmax,SEXP Maxiter,SEXP plot,SEXP best,SEXP better,SEXP random,SEXP bla,SEXP nb_opt_max)
{

  BEGIN_RCPP
  //déclaration des varibles
  NumericVector Z_zi(Zi);
  NumericVector Z_zj(Zj);
  const Map<VectorXd> Z_sj(as<Map<VectorXd> >(Sj));//bic_vide_vect
  const Map<VectorXd> Z_si(as<Map<VectorXd> >(Si));//bic_vide_vect
  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
  const Map<VectorXd> Bic_vide_vect(as<Map<VectorXd> >(bic_vide_vect));//bic_vide_vect

  Rcpp::NumericVector met_tirage(methode_tirage),met_BIC(methode_BIC),rmax(Rmax),maxiter(Maxiter),Plot(plot),Best(best),Better(better),Random(random),Bla(bla),Nb_opt_max(nb_opt_max);
  typedef Rcpp::NumericVector::iterator vec_iterator;
  vec_iterator imet_tirage = met_tirage.begin(),imet_BIC=met_BIC.begin(),irmax=rmax.begin(),imaxiter=maxiter.begin(),iplot=Plot.begin(),ibest=Best.begin(),ibetter=Better.begin(),irandom=Random.begin(),ibla=Bla.begin(),inb_opt_max=Nb_opt_max.begin();
  int p=matX.cols();//nombre de colonne de la matrice Z
  int n=matX.rows();//nombre de lignes de la matrice X
  NumericVector Zj_loc;
  NumericVector Zi_loc;
  NumericVector Zopt_zi;//meilleur Z obtenu
  NumericVector Zopt_zj;//meilleur Z obtenu
  Eigen::VectorXd Zopt_si;//meilleur Z obtenu
  Eigen::VectorXd Zopt_sj;//meilleur Z obtenu
  double Bicbest;//BIC associé au meilleur modèle
  Eigen::VectorXd bicvect;//vecteur BIC des matrices Z retenues
  NumericVector newZ_zi;//matrice Z modifié à chaque étapes
  NumericVector newZ_zj;//matrice Z modifié à chaque étapes
  Eigen::VectorXd newZ_si;//matrice Z modifié à chaque étapes
  Eigen::VectorXd newZ_sj;//matrice Z modifié à chaque étapes
  Eigen::MatrixXd list_cand;//matrice qui contient les coordonnées des candidats
  int nbcand;//nombre de candidats
  int nb_opt;//nombre de fois ou on a retrouve bicbest
  int k;//nombre du tirage aleatoire (liste candidats)
  int compte;//permet de cree la matrice liste en désignant le numéro du candidat (liste candidats)
  int i;//coordonnée (x ou y) des candidats (liste candidats)
  int rand1;//nombre aleatoire pour la 3ème méthode de tirage des candidats (liste candidats)
  int rand2;//nombre aleatoire pour la 3ème méthode de tirage des candidats (liste candidats)
  int numcand;//numero du candidat
  int i_loc;//premiere coordonnée du candidat (modification Z)
  int j_loc;//duexième coordonnée du candidat (modification Z)
  NumericVector Zcand_zi;//matrice Z du candidat
  NumericVector Zcand_zj;//matrice Z du candidat
  Eigen::VectorXd Zcand_si;//matrice Z du candidat
  Eigen::VectorXd Zcand_sj;//matrice Z du candidat
  Eigen::VectorXd BIC_cand;//vecteur qui contient les BIC de chaque colonne des la matrice Zcand (calcul du BIC)
  double Sum_BIC_cand;//somme des BIC de BIC_cand (calcul BIC)
  Eigen::VectorXd stock_bool;//vecteur qui permet de savoir si un candidat peut etre choisi (stock)
  Eigen::VectorXd stock_BIC;//vecteur qui contient le BIC de tout les candidats (stock)
  double sumbic;//BIC de chaque etapes

  Eigen::VectorXd bic_etape ( static_cast<int>(imaxiter[0]) );//vecteur qui stock le BIC de chaque etapes
  Eigen::VectorXd complexite_etape ( static_cast<int>(imaxiter[0]) );//vecteur qui stock la compléxité de Z a chaque etapes
  Eigen::VectorXd etape ( static_cast<int>(imaxiter[0]) );//vecteur qui stock le type de changement de chaque etapes

  int value;
  bool mod;
  int debut_colj;
  int place;
  Eigen::VectorXd zj_loc;
  Eigen::VectorXd zi_loc;
  bool ajout;
  int v;//compteur pour les boucles
  int complexite=0;

  //initialisation
  Zopt_si = VectorXd::Zero(p);
  Zopt_sj = VectorXd::Zero(p);
//somme a la main
  Bicbest = Bic_vide_vect.sum();
  Rcout<<"calcul du BIC";
  bicvect=BICZsparse2_cpp(matX,Z_zi,Z_sj,Bic_vide_vect,imet_BIC[0],n);
//somme a la main
  sumbic=bicvect.sum();
  if (ibla[0]>0)
  {
    Rcout<<sumbic<<"\n";
  }
  if (sumbic<Bicbest)
  {
    Bicbest =sumbic;
    Zopt_zi = Z_zi;
    Zopt_zj = Z_zj;
    Zopt_si = Z_si;
    Zopt_sj = Z_sj;
  }
  if (ibla[0]>0)
  {
   Rcout<<"Bicbest :"<<Bicbest<<"\n";
  }
  newZ_zi=Z_zi;
  newZ_zj=Z_zj;
  newZ_si=Z_si;
  newZ_sj=Z_sj;
  int step =0;
  //Rcout<<"ici";
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
  else if(imet_tirage[0]>0)//methode de tirage aléatoire
  {
    nbcand=imet_tirage[0];
    list_cand.resize(nbcand,2);//le nombre de candidats est determine pas l'utilisateur
  }
  else //si methode a une valeur aberante
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
  stock_bool.resize(nbcand);
  stock_BIC.resize(nbcand);
//  for(v=0;v<newZ_sj.size();v++)
//  {
//    Rcout<<newZ_sj(v)<<" ";
//  }
//  Rcout<<"\n\n";
//  for(v=0;v<newZ_zj.size();v++)
//  {
//    Rcout<<newZ_zj(v)<<" ";
//  }
//
//  Rcout<<"\n";
//  for(v=0;v<newZ_zi.size();v++)
//  {
//    Rcout<<newZ_zi(v)<<" ";
//  }
//
//  Rcout<<"\n\n";
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
      Zj_loc = NumericVector::create();
      Zi_loc = NumericVector::create();

      value=-1;
      mod=true;
      debut_colj=0;
      ajout=false;
      complexite=newZ_zi.size();
      int compl_loc=complexite;
      Zcand_zi=newZ_zi;
      Zcand_zj=newZ_zj;
      Zcand_si=newZ_si;
      Zcand_sj=newZ_sj;

      if(j_loc>0)//si on est pas en premiere colonne
      {
        for(v=0;v<j_loc-1;v++)//on regarde ou commence la colonne j
        {
          debut_colj=debut_colj+Zcand_sj(v);
        }
      }
      place=debut_colj;
      if(Zcand_si(i_loc)!=0 && Zcand_sj(j_loc)!=0)//si la ligne et la colonne du candidats sont non vides
      {
        while(place<(debut_colj+Zcand_sj(j_loc)) )
        {
          if(Zcand_zi(place)==i_loc)//si on a trouve le candidat
          {
            value=place;
            place=complexite+10;//on sort de la boucle
          }
          else if (Zcand_zi(place)>i_loc)//si on a depasse le candidat
          {
            place=complexite+10;//on sort de la boucle
          }
          else//on continue de chercher
          {
            place=place+1;
          }
        }
      }
      if(value==-1)//mode ajout
      {
        if(Zcand_sj(j_loc)<irmax[0])//pas de surcomplexite
        {
          if(Zcand_si(j_loc)!=0 && Zcand_sj(i_loc)!=0)//pas de croisement
          {
            for(v=0;v<compl_loc ;v++)
            {
              if(ajout==false && (Zcand_zj(v)>(j_loc+1) ||(Zcand_zj(v)==(j_loc+1) && Zcand_zi(v)>(i_loc+1)) ))//on ajoute
              {
                Zj_loc.push_back(j_loc+1);//on passe par zj_loc
                Zi_loc.push_back(i_loc+1);
                ajout=true;
              }
              Zj_loc.push_back(Zcand_zj(v));
              Zi_loc.push_back(Zcand_zi(v));
            }
            Zcand_si(i_loc)=Zcand_si(i_loc)+1;
            Zcand_sj(j_loc)=Zcand_sj(j_loc)+1;
            complexite=complexite+1;
            if(ajout==false)//derniere position
            {
              Zj_loc.push_back(j_loc+1);//on passe par zj_loc
              Zi_loc.push_back(i_loc+1);
            }
          }
          else //croisement donc il y'aura une relaxation
          {
            for(v=0;v<compl_loc;v++)
            {
              if(ajout==false && (Zcand_zj(v)>(j_loc+1) ||(Zcand_zj(v)==(j_loc+1)&& Zcand_zi(v)>(i_loc+1)) ))//on ajoute
              {
                Zj_loc.push_back(j_loc+1);//on passe par zj_loc
                Zi_loc.push_back(i_loc+1);
                ajout=true;
              }
              if(Zcand_zj(v)!=(i_loc+1) && Zcand_zi(v)!=(j_loc+1))//si on doit pas supprimer, on recopie
              {
                Zj_loc.push_back(Zcand_zj(v));
                Zi_loc.push_back(Zcand_zi(v));
              }
              else
              {
                Zcand_sj(Zcand_zj(v)-1)=Zcand_sj(Zcand_zj(v)-1)-1;
                Zcand_si(Zcand_zi(v)-1)=Zcand_si(Zcand_zi(v)-1)-1;
                complexite=complexite-1;
              }
            }
            Zcand_si(i_loc)=Zcand_si(i_loc)+1;
            Zcand_sj(j_loc)=Zcand_sj(j_loc)+1;
            complexite=complexite+1;
            if(ajout==false)//derniere position
            {
              Zj_loc.push_back(j_loc+1);//on passe par zj_loc
              Zi_loc.push_back(i_loc+1);
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
        for(v=0;v<compl_loc;v++)
        {
          if(v!=value)
          {
            Zj_loc.push_back(Zcand_zj(v));//on passe par zj_loc
            Zi_loc.push_back(Zcand_zi(v));//on passe par zi_loc
          }
        }
        Zcand_si(i_loc)=Zcand_si(i_loc)-1;
        Zcand_sj(j_loc)=Zcand_sj(j_loc)-1;
        complexite=complexite-1;
      }

      Zcand_zi=Zi_loc;
      Zcand_zj=Zj_loc;

    //calcul du bic (du nouveau Z généré)      (bicZ)
      if (mod==false)
      {
        Sum_BIC_cand=0;
      }
      else
      {
//        if(Zcand_zi.size()!=Zcand_zj.size())
//        {
//          cout<<"pas la meme taille \n";
//        }
//        int compteu=0;
//        int qui=1;
//        if(Zcand_sj.sum()!=Zcand_zi.size())
//        {
//          cout<<"pas la meme taille 2\n\n";
//
//
//          for(v=0;v<newZ_sj.size();v++)
//          {
//            cout<<newZ_sj(v)<<" ";
//          }
//          cout<<"\n\n";
//          for(v=0;v<newZ_zj.size();v++)
//          {
//            cout<<newZ_zj(v)<<" ";
//          }
//                    cout<<"ok1\n";
//
//          cout<<"\n";
//          for(v=0;v<newZ_zi.size();v++)
//          {
//            cout<<newZ_zi(v)<<" ";
//          }
//
//          cout<<"\n\n";
//          cout<<"candidat : "<<j_loc<<" "<<i_loc;
//
//          cout<<"\n\n";
//
//
//          for(v=0;v<Zcand_sj.size();v++)
//          {
//            cout<<Zcand_sj(v)<<" ";
//          }
//          cout<<"\n\n";
//          for(v=0;v<Zcand_zj.size();v++)
//          {
//            cout<<Zcand_zj[v]<<" ";
//          }
//          break;
//        }
//        for(v=0;v<Zcand_zi.size();v++)
//        {
//          if(Zcand_zj(v)==qui)
//          {
//            compteu= compteu+1;
//          }
//          else
//          {
//            if(compteu != Zcand_sj(qui-1))
//            {
//            cout<<"probleme \n";
//            }else
//            {
//              cout<<"ok \n";
//            }
//            compteu=1;
//            qui=Zcand_zj(v);
//          }
//        }
//
//        cout<<"debut BIC 2\n";

        BIC_cand=BICZsparse_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],newZ_sj,n);
//        cout<<"fin BIC \n";
        Sum_BIC_cand=BIC_cand.sum();
      }
    //stockage des valeurs (bic,changement)
      stock_bool(numcand)=mod;
      stock_BIC(numcand)=Sum_BIC_cand;
    }
    //choix du candidat retenu (tirage au sort ou meilleur selon le cas)


    int nb_cand_retenu=stock_bool.sum()+1;//nombre de candidats retenu (certain ne peuvent pas etre pri en compte)
// à faire à la main
//    cout<<"\n"<<"choix du candidat";
    Eigen::VectorXd ind_stock_bool1(nb_cand_retenu-1);//vecteur contenant les numeros des individus retenus
    Eigen::VectorXd stockBIC1(nb_cand_retenu);//vecteur contenant les bic des individus retenus
    int compteur=0;//compteur permattant de remplir stockBIC1 et ind_stock_bool1
    int bettercand=0;
    int iret,jret;//coordonnees du candidat ayant le meilleur bic depuis le debut
    double sumcum=0;
    int w=0;
    double ran;
    Eigen::VectorXd bicweight(nb_cand_retenu);
    double sumexp=0;
    double BIC_min=sumbic;

    stockBIC1(0)=sumbic;
    for (i=0;i<stock_BIC.size();i++)//on garde que les individus retunus
    {
      if(stock_bool(i,0)==1)
      {
        ind_stock_bool1(compteur)=i;//seulement les individus possibles
        stockBIC1(compteur+1)=stock_BIC(i);
        if(stockBIC1(compteur+1)<BIC_min)//on cherche le plus petit BIC
        {
          BIC_min=stockBIC1(compteur+1);
          bettercand=compteur+1;//+1 a cause de la stationarité
        }
        compteur=compteur+1;
      }
    }

    //prise de décision
    if(BIC_min<Bicbest) //regarde s'il y'a un BIC meilleur que le meuilleur rencontré
    {
      nb_opt=0;//il y a un nouveau bicbest donc le compteur revient a 0
      Bicbest=BIC_min;
      iret=list_cand(ind_stock_bool1(bettercand-1),0);//-1 a cause de la stationarité
      jret=list_cand(ind_stock_bool1(bettercand-1),1);
      Zj_loc = NumericVector::create();
      Zi_loc = NumericVector::create();
      Zopt_zi = newZ_zi;//initialisation à la stationarité
      Zopt_zj = newZ_zj;
      Zopt_si = newZ_si;
      Zopt_sj = newZ_sj;
      value=-1;
      debut_colj=0;
      ajout=false;
      complexite=newZ_zi.size();
      int compl_opt=complexite;

      //debut modification zopt(i,j)=1-zopt(i,j)
      if(jret>0)//si on est pas en premiere colonne
      {
        for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
        {
          debut_colj=debut_colj+Zopt_sj(k);
        }
      }
      place=debut_colj;

      if(Zopt_si(iret)!=0 && Zopt_sj(jret)!=0)
      {
        while(place<(debut_colj+Zopt_sj(jret)) )
        {
          if(Zopt_zi(place)==iret)
          {
            value=place;
            place=complexite+10;//on sort de la boucle
          }
          else if (Zopt_zi(place)>iret)
          {
            place=complexite+10;//on sort de la boucle
          }
          else//on continue de chercher
          {
            place=place+1;
          }
        }
      }
      if(value==-1)//mode ajout
      {
        if(Zopt_si(jret)!=0 && Zopt_sj(iret)!=0)//pas de croisement
        {
          for(k=0;k<compl_opt;k++)
          {
            if(ajout==false && (Zopt_zj(k)>(jret+1) ||(Zopt_zj(k)==(jret+1) && Zopt_zi(k)>(iret+1)) ))//on ajoute
            {
              Zj_loc.push_back(jret+1);//on passe par zj_loc
              Zi_loc.push_back(iret+1);
              ajout=true;
            }
            Zj_loc.push_back(Zopt_zj(k));//on passe par zj_loc
            Zi_loc.push_back(Zopt_zi(k));
          }
          Zopt_si(iret)=Zopt_si(iret)+1;
          Zopt_sj(jret)=Zopt_sj(jret)+1;
          complexite=complexite+1;
          if(ajout==false)//derniere position
          {
            Zj_loc.push_back(jret+1);//on passe par zj_loc
            Zi_loc.push_back(iret+1);
          }
        }
        else //croisement
        {
          for(k=0;k<compl_opt;k++)
          {
            if(ajout==false && (Zopt_zj(k)>(jret+1) ||(Zopt_zj(k)==(jret+1) && Zopt_zi(k)>(iret+1)) ))//on ajoute
            {
              Zj_loc.push_back(jret+1);//on passe par zj_loc
              Zi_loc.push_back(iret+1);
              ajout=true;
            }
            if(Zopt_zj(k)!=(iret+1) && Zopt_zi(k)!=(jret+1))//si on doit pas supprimer, on recopie
            {
              Zj_loc.push_back(Zopt_zj(k));//on passe par zj_loc
              Zi_loc.push_back(Zopt_zi(k));
            }
            else
            {
              Zopt_sj(Zopt_zj(k)-1)=Zopt_sj(Zopt_zj(k)-1)-1;
              Zopt_si(Zopt_zi(k)-1)=Zopt_si(Zopt_zi(k)-1)-1;
              complexite=complexite-1;
            }
          }
          Zopt_si(iret)=Zopt_si(iret)+1;
          Zopt_sj(jret)=Zopt_sj(jret)+1;
          complexite=complexite+1;
          if(ajout==false)//derniere position
          {
            Zj_loc.push_back(jret+1);//on passe par zj_loc
            Zi_loc.push_back(iret+1);
          }
        }
      }
      else//suppression
      {
        for(k=0;k<compl_opt;k++)
        {
          if(k!=value)
          {
            Zj_loc.push_back(Zopt_zj(k));//on passe par zj_loc
            Zi_loc.push_back(Zopt_zi(k));//on passe par zi_loc
          }
        }
        Zopt_si(iret)=Zopt_si(iret)-1;
        Zopt_sj(jret)=Zopt_sj(jret)-1;
        complexite=complexite-1;
      }

      Zopt_zi=Zi_loc;
      Zopt_zj=Zj_loc;

//      cout<<Zopt_zi<<"\n"<<Zopt_zj<<"\n";
      //fin de la modifidation zopt(i,j)=1-zopt(i,j)

      if (ibla[0]>0)//commentaires
      {
        Rcout<<step<<" Bicbest: "<<Bicbest<<" complexite: "<<complexite<<"\n";
        if (ibla[0]==2)
        {
          Rcout<<"nb_cand "<<nb_cand_retenu<<"BIC min "<<BIC_min<<"\n";
        }
      }

      if(ibest[0]==1)//on a vu le meilleur absolu donc si on veut y aller, on y va
      {
        bicvect=BICZsparse_cpp(matX,Zopt_zi,Zopt_sj,Bic_vide_vect,bicvect,imet_BIC[0],newZ_sj,n);
        newZ_zi=Zopt_zi;
        newZ_zj=Zopt_zj;
        newZ_si=Zopt_si;
        newZ_sj=Zopt_sj;
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
        ran=runif(0,1)[0];
        //ran=ran/32767;
        sumcum=0;
        while (ran>=sumcum)//choix du candidat
        {
          w++;
          bicweight(w)=bicweight(w)/sumexp;
          sumcum=sumcum+bicweight(w);
        }
        if(w==0)
        {
  //       cout<<"on reste au meme endroit";
        }
        else
        {
          iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
          jret=list_cand(ind_stock_bool1(w-1),1);
          Zj_loc = NumericVector::create();
          Zi_loc = NumericVector::create();
          Zcand_zi=newZ_zi;
          Zcand_zj=newZ_zj;
          Zcand_si=newZ_si;
          Zcand_sj=newZ_sj;
          value=-1;
          debut_colj=0;
          ajout=false;
          complexite=newZ_zi.size();
          int compl_cand=complexite;

          //debut de la modification Zcand(i,j)=1-Zcand(i,j)
          if(jret>0)//si on est pas en premiere colonne
          {
            for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
            {
              debut_colj=debut_colj+Zcand_sj(k);
            }
          }
          place=debut_colj;

          if(Zcand_si(iret)!=0 && Zcand_sj(jret)!=0)
          {
            while(place<(debut_colj+Zcand_sj(jret)) )
            {
              if(Zcand_zi(place)==iret)
              {
                value=place;
                place=complexite+10;//on sort de la boucle
              }
              else if (Zcand_zi(place)>iret)
              {
                place=complexite+10;//on sort de la boucle
              }
              else//on continue de chercher
              {
                place=place+1;
              }
            }
          }
          if(value==-1)//mode ajout
          {
            if(Zcand_si(jret)!=0 && Zcand_sj(iret)!=0)//pas de croisement
            {
              for(k=0;k<compl_cand;k++)
              {
                if(ajout==false && (Zcand_zj(k)>(jret+1) ||(Zcand_zj(k)==(jret+1) && Zcand_zi(k)>(iret+1)) ))//on ajoute
                {
                  Zj_loc.push_back(jret+1);//on passe par zj_loc
                  Zi_loc.push_back(iret+1);
                  ajout=true;
                }
                Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
                Zi_loc.push_back(Zcand_zi(k));
              }
              Zcand_si(iret)=Zcand_si(iret)+1;
              Zcand_sj(jret)=Zcand_sj(jret)+1;
              complexite=complexite+1;
              if(ajout==false)//derniere position
              {
                Zj_loc.push_back(jret+1);//on passe par zj_loc
                Zi_loc.push_back(iret+1);
              }
            }
            else //croisement
            {
              for(k=0;k<compl_cand;k++)
              {
                if(ajout==false && (Zcand_zj(k)>(jret+1) ||(Zcand_zj(k)==(jret+1) && Zcand_zi(k)>(iret+1)) ))//on ajoute
                {
                  Zj_loc.push_back(jret+1);//on passe par zj_loc
                  Zi_loc.push_back(iret+1);
                  ajout=true;
                }
                if(Zcand_zj(k)!=(iret+1) && Zcand_zi(k)!=(jret+1))//si on doit pas supprimer, on recopie
                {
                  Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
                  Zi_loc.push_back(Zcand_zi(k));
                }
                else
                {
                  Zcand_sj(Zcand_zj(k)-1)=Zcand_sj(Zcand_zj(k)-1)-1;
                  Zcand_si(Zcand_zi(k)-1)=Zcand_si(Zcand_zi(k)-1)-1;
                  complexite=complexite-1;
                }
              }
              Zcand_si(iret)=Zcand_si(iret)+1;
              Zcand_sj(jret)=Zcand_sj(jret)+1;
              complexite=complexite+1;
              if(ajout==false)//derniere position
              {
                Zj_loc.push_back(jret+1);//on passe par zj_loc
                Zi_loc.push_back(iret+1);
              }
            }
          }
          else//suppression
          {
            for(k=0;k<compl_cand;k++)
            {
              if(k!=value)
              {
                Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
                Zi_loc.push_back(Zcand_zi(k));//on passe par zi_loc
              }
            }
            Zcand_si(iret)=Zcand_si(iret)-1;
            Zcand_sj(jret)=Zcand_sj(jret)-1;
            complexite=complexite-1;
          }

          Zcand_zi=Zi_loc;
          Zcand_zj=Zj_loc;
//          cout<<Zcand_zi<<"\n"<<Zcand_zj<<"\n";

          bicvect=BICZsparse_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],newZ_sj,n);
          newZ_zi=Zcand_zi;
          newZ_zj=Zcand_zj;
          newZ_si=Zcand_si;
          newZ_sj=Zcand_sj;
          sumbic=stockBIC1(w);
        }
      }
    }
    else if(nb_cand_retenu==1) //si la stationarité est le seul candidat
    {
//      cout<<"on reste au meme endroit";
    }
    else if(ibetter[0]==1 && bettercand!=0)//si on a un meilleur local et qu'on veut y aller, on y va
    {
      iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
      jret=list_cand(ind_stock_bool1(w-1),1);
      Zj_loc = NumericVector::create();
      Zi_loc = NumericVector::create();
      Zcand_zi=newZ_zi;
      Zcand_zj=newZ_zj;
      Zcand_si=newZ_si;
      Zcand_sj=newZ_sj;
      value=-1;
      debut_colj=0;
      ajout=false;
      complexite=newZ_zi.size();
      int compl_cand=complexite;

      //debut de la modification Zcand(i,j)=1-Zcand(i,j)
      if(jret>0)//si on est pas en premiere colonne
      {
        for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
        {
          debut_colj=debut_colj+Zcand_sj(k);
        }
      }
      place=debut_colj;

      if(Zcand_si(iret)!=0 && Zcand_sj(jret)!=0)
      {
        while(place<(debut_colj+Zcand_sj(jret)) )
        {
          if(Zcand_zi(place)==iret)
          {
            value=place;
            place=complexite+10;//on sort de la boucle
          }
          else if (Zcand_zi(place)>iret)
          {
            place=complexite+10;//on sort de la boucle
          }
          else//on continue de chercher
          {
            place=place+1;
          }
        }
      }
      if(value==-1)//mode ajout
      {
        if(Zcand_si(jret)!=0 && Zcand_sj(iret)!=0)//pas de croisement
        {
          for(k=0;k<compl_cand;k++)
          {
            if(ajout==false && (Zcand_zj(k)>(jret+1) ||(Zcand_zj(k)==(jret+1) && Zcand_zi(k)>(iret+1)) ))//on ajoute
            {
              Zj_loc.push_back(jret+1);//on passe par zj_loc
              Zi_loc.push_back(iret+1);
              ajout=true;
            }
            Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
            Zi_loc.push_back(Zcand_zi(k));
          }
          Zcand_si(iret)=Zcand_si(iret)+1;
          Zcand_sj(jret)=Zcand_sj(jret)+1;
          complexite=complexite+1;
          if(ajout==false)//derniere position
          {
            Zj_loc.push_back(jret+1);//on passe par zj_loc
            Zi_loc.push_back(iret+1);
          }
        }
        else //croisement
        {
          for(k=0;k<compl_cand;k++)
          {
            if(ajout==false && (Zcand_zj(k)>(jret+1) ||(Zcand_zj(k)==(jret+1) && Zcand_zi(k)>(iret+1)) ))//on ajoute
            {
              Zj_loc.push_back(jret+1);//on passe par zj_loc
              Zi_loc.push_back(iret+1);
              ajout=true;
            }
            if(Zcand_zj(k)!=(iret+1) && Zcand_zi(k)!=(jret+1))//si on doit pas supprimer, on recopie
            {
              Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
              Zi_loc.push_back(Zcand_zi(k));
            }
            else
            {
              Zcand_sj(Zcand_zj(k)-1)=Zcand_sj(Zcand_zj(k)-1)-1;
              Zcand_si(Zcand_zi(k)-1)=Zcand_si(Zcand_zi(k)-1)-1;
              complexite=complexite-1;
            }
          }
          Zcand_si(iret)=Zcand_si(iret)+1;
          Zcand_sj(jret)=Zcand_sj(jret)+1;
          complexite=complexite+1;
          if(ajout==false)//derniere position
          {
            Zj_loc.push_back(jret+1);//on passe par zj_loc
            Zi_loc.push_back(iret+1);
          }
        }
      }
      else//suppression
      {
        for(k=0;k<compl_cand;k++)
        {
          if(k!=value)
          {
            Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
            Zi_loc.push_back(Zcand_zi(k));//on passe par zi_loc
          }
        }
        Zcand_si(iret)=Zcand_si(iret)-1;
        Zcand_sj(jret)=Zcand_sj(jret)-1;
        complexite=complexite-1;
      }

      Zcand_zi=Zi_loc;
      Zcand_zj=Zj_loc;

//      cout<<Zcand_zi<<"\n"<<Zcand_zj<<"\n";

      bicvect=BICZsparse_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],newZ_sj,n);
      newZ_zi=Zcand_zi;
      newZ_zj=Zcand_zj;
      newZ_si=Zcand_si;
      newZ_sj=Zcand_sj;
      sumbic=BIC_min;
    }
    else if (irandom[0]==0)
    {
//      station=true;
    }
    else//tirage au sort (on a pas de saut systématique)
    {
      bicweight=stockBIC1;
      for (v=0;v<nb_cand_retenu;v++)
      {
        bicweight(v)=bicweight(v)-BIC_min;
        bicweight(v)=exp(-bicweight(v)/2);
        sumexp=sumexp+bicweight(v);
      }

      w=-1;//initialisation candidat choisi (il est a -1 car on va rajouter un "+1" avec le while)
          ran=runif(0,1)[0];
        //ran=ran/32767;
      sumcum=0;
      while (ran>=sumcum)//choix du candidat
      {
        w++;
        bicweight(w)=bicweight(w)/sumexp;
        sumcum=sumcum+bicweight(w);
      }
      if(w==0)
      {
//       cout<<"on reste au meme endroit";
//       station=true;
      }
      else
      {
        iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
        jret=list_cand(ind_stock_bool1(w-1),1);
        Zj_loc = NumericVector::create();
        Zi_loc = NumericVector::create();
        Zcand_zi=newZ_zi;
        Zcand_zj=newZ_zj;
        Zcand_si=newZ_si;
        Zcand_sj=newZ_sj;
        value=-1;
        debut_colj=0;
        ajout=false;
        complexite=newZ_zi.size();
        int compl_cand=complexite;
        //debut de la modification Zcand(i,j)=1-Zcand(i,j)
        if(jret>0)//si on est pas en premiere colonne
        {
          for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
          {
            debut_colj=debut_colj+Zcand_sj(k);
          }
        }
        place=debut_colj;

        if(Zcand_si(iret)!=0 && Zcand_sj(jret)!=0)
        {
          while(place<(debut_colj+Zcand_sj(jret)) )
          {
            if(Zcand_zi(place)==iret)
            {
              value=place;
              place=complexite+10;//on sort de la boucle
            }
            else if (Zcand_zi(place)>iret)
            {
              place=complexite+10;//on sort de la boucle
            }
            else//on continue de chercher
            {
              place=place+1;
            }
          }
        }
        if(value==-1)//mode ajout
        {
          if(Zcand_si(jret)!=0 && Zcand_sj(iret)!=0)//pas de croisement
          {
            for(k=0;k<compl_cand;k++)
            {
              if(ajout==false && (Zcand_zj(k)>(jret+1) ||(Zcand_zj(k)==(jret+1) && Zcand_zi(k)>(iret+1)) ))//on ajoute
              {
                Zj_loc.push_back(jret+1);//on passe par zj_loc
                Zi_loc.push_back(iret+1);
                ajout=true;
              }
              Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
              Zi_loc.push_back(Zcand_zi(k));
            }
            Zcand_si(iret)=Zcand_si(iret)+1;
            Zcand_sj(jret)=Zcand_sj(jret)+1;
            complexite=complexite+1;
            if(ajout==false)//derniere position
            {
              Zj_loc.push_back(jret+1);//on passe par zj_loc
              Zi_loc.push_back(iret+1);
            }
          }
          else //croisement
          {
            for(k=0;k<compl_cand;k++)
            {
              if(ajout==false && (Zcand_zj(k)>(jret+1) ||(Zcand_zj(k)==(jret+1) && Zcand_zi(k)>(iret+1)) ))//on ajoute
              {
                Zj_loc.push_back(jret+1);//on passe par zj_loc
                Zi_loc.push_back(iret+1);
                ajout=true;
              }
              if(Zcand_zj(k)!=(iret+1) && Zcand_zi(k)!=(jret+1))//si on doit pas supprimer, on recopie
              {
                Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
                Zi_loc.push_back(Zcand_zi(k));
              }
              else
              {
                Zcand_sj(Zcand_zj(k)-1)=Zcand_sj(Zcand_zj(k)-1)-1;
                Zcand_si(Zcand_zi(k)-1)=Zcand_si(Zcand_zi(k)-1)-1;
                complexite=complexite-1;
              }
            }
            Zcand_si(iret)=Zcand_si(iret)+1;
            Zcand_sj(jret)=Zcand_sj(jret)+1;
            complexite=complexite+1;
            if(ajout==false)//derniere position
            {
              Zj_loc.push_back(jret+1);//on passe par zj_loc
              Zi_loc.push_back(iret+1);
            }
          }
        }
        else//suppression
        {
          for(k=0;k<compl_cand;k++)
          {
            if(k!=value)
            {
              Zj_loc.push_back(Zcand_zj(k));//on passe par zj_loc
              Zi_loc.push_back(Zcand_zi(k));//on passe par zi_loc
            }
          }
          Zcand_si(iret)=Zcand_si(iret)-1;
          Zcand_sj(jret)=Zcand_sj(jret)-1;
          complexite=complexite-1;
        }

        Zcand_zi=Zi_loc;
        Zcand_zj=Zj_loc;

//        cout<<Zcand_zi<<"\n"<<Zcand_zj<<"\n";

        bicvect=BICZsparse_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],newZ_sj,n);
        newZ_zi=Zcand_zi;
        newZ_zj=Zcand_zj;
        newZ_si=Zcand_si;
        newZ_sj=Zcand_sj;
        sumbic=stockBIC1(w);
      }
    }

    if(sumbic==Bicbest)//on regarde si le le BIC est le meme que le BIC optimal
    {
      nb_opt=nb_opt+1;//si c'est le cas alors on a retrouve une fois de plus BICbest
      if(nb_opt==inb_opt_max[0])//si on a atteint le nombre maximum de fois ou on a retrouvé BICbest
      {
        step=imaxiter[0];//alors on sort du while
        Rcout<<"convergence reached";
      }
    }


    if(iplot[0]==1)//si on veut des graphiques on mets à jour les vecteurs
    {
      bic_etape (step)=sumbic;
      complexite_etape (step)=complexite;
//      if(relax_effect==true)
//      {
//        type_changement_etape=3;
//      }
//      if (station==true)
//      {
//        etape (step)=2;//stationarité
//      }
//      else if(newZ(iret,jret)==0)
//      {
//        etape (step)=0;//suppression
//      }
//      else
//      {
//        etape (step)=1;//ajout
//      }
    }
    if(ibla[0]>2)//commentaires lors des etapes
    {
      Rcout<<step<<" local BIC: "<<sumbic<<" local complexity: "<<complexite<<"\n";
    }
    step=step+1;
  }//fin des étapes (while)

  if(iplot[0]==1)
  {
    return List::create(
      Named("newZ_zi")=  newZ_zi,
      Named("newZ_zj")=  newZ_zj,
      Named("newZ_si")=  newZ_si,
      Named("newZ_sj")=  newZ_sj,
      Named("bic_loc")=  sumbic,
      Named("Zopt_zi")=  Zopt_zi,
      Named("Zopt_zj")=  Zopt_zj,
      Named("Zopt_si")=  Zopt_si,
      Named("Zopt_sj")=  Zopt_sj,

      Named("bic_opt")=  Bicbest,
      Named("bic_step")=  bic_etape,
      Named("complexity_step")=  complexite_etape,
      Named("step")=  etape
    );
  }
  else
  {
  return List::create(
      Named("newZ_zi")=  newZ_zi,
      Named("newZ_zj")=  newZ_zj,
      Named("newZ_si")=  newZ_si,
      Named("newZ_sj")=  newZ_sj,
      Named("bic_loc")=  sumbic,
      Named("Zopt_zi")=  Zopt_zi,
      Named("Zopt_zj")=  Zopt_zj,
      Named("Zopt_si")=  Zopt_si,
      Named("Zopt_sj")=  Zopt_sj,
      Named("bic_opt")=  Bicbest
    );
  }
  END_RCPP
}
