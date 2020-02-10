//#include "rechercheZ_sparse_rejet.h"
//#include "BICZsparse_j_cpp.h"
//#include "BICZsparse2_cpp.h"
//#include <iostream>
//#include <string>
//#include <math.h>
//using namespace Rcpp ;
//using namespace Eigen;
//using namespace std;
//using Eigen::Map;
//using Eigen::MatrixXd;
//using Rcpp::as;
//
//SEXP rechercheZ_sparse_rejet(SEXP X,SEXP Zi,SEXP Zj,SEXP Si,SEXP Sj,SEXP bic_vide_vect,SEXP methode_tirage,SEXP methode_BIC,SEXP Rmax,SEXP Maxiter,SEXP plot,SEXP best,SEXP better,SEXP random,SEXP bla,SEXP nb_opt_max)
//{
// 
//  BEGIN_RCPP
//  //déclaration des varibles
//  const Map<VectorXd> Z_zi(as<Map<VectorXd> >(Zi));//bic_vide_vect
//  const Map<VectorXd> Z_zj(as<Map<VectorXd> >(Zj));//bic_vide_vect
//  const Map<VectorXd> Z_sj(as<Map<VectorXd> >(Sj));//bic_vide_vect
//  const Map<VectorXd> Z_si(as<Map<VectorXd> >(Si));//bic_vide_vect
//  const Map<MatrixXd> matX(as<Map<MatrixXd> >(X));//X
//  const Map<VectorXd> Bic_vide_vect(as<Map<VectorXd> >(bic_vide_vect));//bic_vide_vect
//  
//  Rcpp::NumericVector met_tirage(methode_tirage),met_BIC(methode_BIC),rmax(Rmax),maxiter(Maxiter),Plot(plot),Best(best),Better(better),Random(random),Bla(bla),Nb_opt_max(nb_opt_max);
//  typedef Rcpp::NumericVector::iterator vec_iterator;
//  vec_iterator imet_tirage = met_tirage.begin(),imet_BIC=met_BIC.begin(),irmax=rmax.begin(),imaxiter=maxiter.begin(),iplot=Plot.begin(),ibest=Best.begin(),ibetter=Better.begin(),irandom=Random.begin(),ibla=Bla.begin(),inb_opt_max=Nb_opt_max.begin();
//  int p=matX.cols();//nombre de colonne de la matrice Z
//  int n=matX.rows();//nombre de lignes de la matrice X
//  Eigen::VectorXd Zopt_zi;//meilleur Z obtenu
//  Eigen::VectorXd Zopt_zj;//meilleur Z obtenu
//  Eigen::VectorXd Zopt_si;//meilleur Z obtenu
//  Eigen::VectorXd Zopt_sj;//meilleur Z obtenu
//  double Bicbest;//BIC associé au meilleur modèle
//  Eigen::VectorXd bicvect;//vecteur BIC des matrices Z retenues
//  Eigen::VectorXd newZ_zi;//matrice Z modifié à chaque étapes
//  Eigen::VectorXd newZ_zj;//matrice Z modifié à chaque étapes
//  Eigen::VectorXd newZ_si;//matrice Z modifié à chaque étapes
//  Eigen::VectorXd newZ_sj;//matrice Z modifié à chaque étapes  
//  Eigen::MatrixXd list_cand;//matrice qui contient les coordonnées des candidats
//  int nbcand;//nombre de candidats
//  int nb_opt;//nombre de fois ou on a retrouve bicbest
//  int k;//nombre du tirage aleatoire (liste candidats)
//  int compte;//permet de cree la matrice liste en désignant le numéro du candidat (liste candidats)
//  int i;//coordonnée (x ou y) des candidats (liste candidats)
//  int rand1;//nombre aleatoire pour la 3ème méthode de tirage des candidats (liste candidats)
//  int rand2;//nombre aleatoire pour la 3ème méthode de tirage des candidats (liste candidats)
//  int numcand;//numero du candidat
//  int i_loc;//premiere coordonnée du candidat (modification Z)
//  int j_loc;//duexième coordonnée du candidat (modification Z)
//  Eigen::VectorXd Zcand_zi;//matrice Z du candidat
//  Eigen::VectorXd Zcand_zj;//matrice Z du candidat
//  Eigen::VectorXd Zcand_si;//matrice Z du candidat
//  Eigen::VectorXd Zcand_sj;//matrice Z du candidat
//  Eigen::VectorXd BIC_cand;//vecteur qui contient les BIC de chaque colonne des la matrice Zcand (calcul du BIC)
//  double Sum_BIC_cand;//somme des BIC de BIC_cand (calcul BIC)
//  Eigen::VectorXd stock_bool;//vecteur qui permet de savoir si un candidat peut etre choisi (stock)
//  Eigen::VectorXd stock_BIC;//vecteur qui contient le BIC de tout les candidats (stock)
//  double sumbic;//BIC de chaque etapes
//  
//  Eigen::VectorXd bic_etape (imaxiter[0]);//vecteur qui stock le BIC de chaque etapes
//  Eigen::VectorXd complexite_etape (imaxiter[0]);//vecteur qui stock la compléxité de Z a chaque etapes
//  Eigen::VectorXd etape (imaxiter[0]);//vecteur qui stock le type de changement de chaque etapes
//  bool station;//permet de savoir si on est stationnaire ou non
//  bool relax_effect;
//        
//  int value;
//  bool mod;
//  int debut_colj;
//  int place;
//  Eigen::VectorXd zj_loc;
//  Eigen::VectorXd zi_loc;
//  bool ajout;
//  int v;//compteur pour les boucles
//  int complexite;
//    
//  //initialisation
//  Zopt_si = VectorXd::Zero(p);
//  Zopt_sj = VectorXd::Zero(p);
////somme a la main
//  Bicbest = Bic_vide_vect.sum();
//  bicvect=BICZsparse2_cpp(matX,Z_zi,Z_sj,Bic_vide_vect,imet_BIC[0],n);
////somme a la main
//  sumbic=bicvect.sum();
//  if (ibla[0]>0)
//  {
//    cout<<sumbic<<"\n";
//  }
//  if (sumbic<Bicbest)
//  {
//    Bicbest =sumbic;
//    Zopt_zi = Z_zi;
//    Zopt_zj = Z_zj;
//    Zopt_si = Z_si;
//    Zopt_sj = Z_sj;
//  }
//  newZ_zi=Z_zi;
//  newZ_zj=Z_zj;
//  newZ_si=Z_si;
//  newZ_sj=Z_sj;
//  int step =0;
//  
//  if (imet_tirage[0]==0)//methode de changement de la ligne et de la colonne
//  {
//    nbcand=2*p-2;
//    list_cand.resize(nbcand,2);//2p-2 candidats
//  }
//  else if(imet_tirage[0]==-1)
//  {
//    nbcand=p-1;
//    list_cand.resize(nbcand,2);//p-1 candidats (colonne uniquement)
//  }
//  else if(imet_tirage[0]>0)//methode de tirage aléatoire
//  {
//    nbcand=imet_tirage[0];
//    list_cand.resize(nbcand,2);//le nombre de candidats est determine pas l'utilisateur
//  }
//  else //si methode a une valeur aberante
//  {
//    throw std::range_error("methode de tirage incorrecte");
//  }
//  
//  if (irandom[0]==0)
//  {
//    ibetter[0]=1;
//    ibest[0]=1;
//  }
//  else if (ibetter[0]==1)
//  {
//    ibest[0]=1;
//  }
//  
//  nb_opt=0;
//  
//  while(step<imaxiter[0])
//  // initialisation des variables
//  
//  {
//    compte=0;//initialisation du vecteur liste désigne le numéro du candidat 
//  //liste candidats (couples [i,j])
//    if (imet_tirage[0]==0)//methode de changement de la ligne et de la colonne
//    {
//      k=rand() % p;//on tire un numero aleatoire pour savoir quel est la ligne et la colonne de candidats
//      for(i=0;i<p;i++)
//      {
//        if(i!=k)//on ne veut pas avoir de 1 sur la diagonale
//        {
//          list_cand(compte,0)=i;
//          list_cand(compte,1)=k;
//          compte=compte+1;
//          list_cand(compte,0)=k;
//          list_cand(compte,1)=i;
//          compte=compte+1;
//        }
//      }    
//    }
//    else if (imet_tirage[0]==-1)//seulement la colonne
//    {
//      k=rand() % p;
//      for(i=0;i<p;i++)
//      {
//        if(i!=k)
//          {
//            list_cand(compte,0)=i;
//            list_cand(compte,1)=k;
//            compte=compte+1;
//          }
//      }
//    }
//    else if (imet_tirage[0]>0)//methode de tirage aléatoire
//    {
//      for(i=0;i<nbcand;i++)
//      {
//        rand1=rand() % p;//nombres aleatoire pour avoir le numero de la ligne
//        rand2=rand() % p;//nombres aleatoire pour avoir le numero de la colonne
//        if (rand1==rand2)//on ne veut pas de 1 sur la diagonale
//        {
//          if(rand1<p-1)
//          {
//            rand2=rand1+1;
//          }
//          else
//          {
//            rand2=rand1-1;
//          }
//        }
//        list_cand(i,0)=rand1;
//        list_cand(i,1)=rand2;            
//      }
//    }
//  
//    
//    //pour chaque candidat
//    for (numcand=0;numcand<nbcand;numcand++)
//    {
//    //modification (calcul du Z)    (Zcand avec methode (rejet ou relax)   Z,i,j,methode="relax", p2max=inf,rmax=inf)
//      i_loc=list_cand(numcand,0);
//      j_loc=list_cand(numcand,1);
//      value=-1;
//      mod=true;
//      debut_colj=0;
//      ajout=false;
//      complexite=newZ_zi.size();
//      Zcand_zi=newZ_zi;
//      Zcand_zj=newZ_zj;
//      Zcand_si=newZ_si;
//      Zcand_sj=newZ_sj;
//      
//      if(j_loc>1)//si on est pas en premiere colonne
//      {
//        for(v=0;v<j_loc-1;v++)//on regarde ou commence la colonne j
//        {
//          debut_colj=debut_colj+Zcand_sj(v);
//        }
//      }
//      place=debut_colj;
//      
//      if(Zcand_si(i_loc-1)!=0 && Zcand_sj(j_loc-1)!=0)
//      {
//        while(place<(debut_colj+Zcand_sj(j_loc-1)) )
//        {
//          if(Zcand_zi(place)==i_loc)
//          {
//            value=place;
//            place=complexite+10;//on sort de la boucle
//          }
//          else if (Zcand_zi(place)>i_loc)
//          {
//            place=complexite+10;//on sort de la boucle
//          }
//          else//on continue de chercher
//          {
//            place=place+1;
//          }
//        }
//      }
//      if(value==-1)//mode ajout
//      {
//        if(Zcand_sj(j_loc-1)<irmax[0])//pas de surcomplexite
//        {
//          if(Zcand_si(j_loc-1)!=0 && Zcand_sj(i_loc-1)!=0)//pas de croisement
//          {
//            for(v=0;v<complexite;v++)
//            {
//              if(ajout==false && (Zcand_zj(v)>j_loc ||(Zcand_zj(v)==j_loc && Zcand_zi(v)>i_loc) ))//on ajoute
//              {            
//                zj_loc<<zj_loc,j_loc;//on passe par zj_loc
//                zi_loc<<zi_loc,i_loc;
//                ajout=true;
//              }
//              zj_loc<<zj_loc,Zcand_zj(v);
//              zi_loc<<zi_loc,Zcand_zi(v);
//            }
//            Zcand_zi=zi_loc;
//            Zcand_zj=zj_loc;
//            Zcand_si(i_loc-1)=Zcand_si(i_loc-1)+1;
//            Zcand_sj(j_loc-1)=Zcand_sj(j_loc-1)+1;
//            complexite=complexite+1;
//            if(ajout==false)//derniere position
//            {
//              Zcand_zj<<Zcand_zj,j_loc;
//              Zcand_zi<<Zcand_zi,i_loc;
//            }
//          }
//          else //croisement donc on rejette
//          {
//            mod=false;
//          }
//        }
//        else //surcomplexite
//        {
//          mod=false;
//        }
//      }
//      else//suppression
//      {
//        for(v=0;v<complexite;v++)
//        {
//          if(v!=value)
//          {
//            zj_loc(v)=Zcand_zj(v);
//            zi_loc(v)=Zcand_zi(v);
//          }
//        }
//        Zcand_zi=zi_loc;
//        Zcand_zj=zj_loc;
//        Zcand_si(i_loc-1)=Zcand_si(i_loc-1)-1;
//        Zcand_sj(j_loc-1)=Zcand_sj(j_loc-1)-1;
//        complexite=complexite-1;
//      }
//    //calcul du bic (du nouveau Z généré)      (bicZ)
//      if (mod==0)
//      {
//        Sum_BIC_cand=0;
//      }
//      else
//      {
//        BIC_cand=BICZsparse_j_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],n,j_loc);
//        Sum_BIC_cand=BIC_cand.sum();
//        
//      }
//    //stockage des valeurs (bic,changement)
//      stock_bool.resize(nbcand);
//      stock_bool(numcand)=mod;
//      stock_BIC.resize(nbcand);
//      stock_BIC(numcand)=Sum_BIC_cand;
//    }
//    //choix du candidat retenu (tirage au sort ou meilleur selon le cas)
//    
//
//    int nb_cand_retenu=stock_bool.sum()+1;//nombre de candidats retenu (certain ne peuvent pas etre pri en compte)
//// à faire à la main
//
//    Eigen::VectorXd ind_stock_bool1(nb_cand_retenu-1);//vecteur contenant les numeros des individus retenus
//    Eigen::VectorXd stockBIC1(nb_cand_retenu);//vecteur contenant les bic des individus retenus
//    int compteur=0;//compteur permattant de remplir stockBIC1 et ind_stock_bool1
//    int bestloc=0;//booleen permettant de voir si on trouve un meilleur bic que le meilleur bic actuel
//    int bettercand=0;
//    int iret,jret;//coordonnees du candidat ayant le meilleur bic depuis le debut
//    double sumcum=0;
//    int w;
//    double ran;
//    Eigen::VectorXd bicweight(nb_cand_retenu);
//    double sumexp=0;
//    double BIC_min=sumbic;
//    station=false;
//    relax_effect=false;
//    
//    stockBIC1(0)=sumbic;
//    for (i=0;i<stock_BIC.size();i++)//on garde que les individus retunus
//    {
//      if(stock_bool(i,0)==1)
//      {
//        ind_stock_bool1(compteur)=i;//seulement les individus possibles
//        stockBIC1(compteur+1)=stock_BIC(i);
//        if(stockBIC1(compteur+1)<BIC_min)//on cherche le plus petit BIC
//        {
//          BIC_min=stockBIC1(compteur+1);
//          bettercand=compteur+1;//+1 a cause de la stationarité
//        }
//        compteur=compteur+1;
//      }     
//    } 
//    
//    //prise de décision
//    if(BIC_min<Bicbest) //regarde s'il y'a un BIC meilleur que le meuilleur rencontré
//    {
//      nb_opt=0;//il y a un nouveau bicbest donc le compteur revient a 0
//      bestloc=1;//facultatif
//      Bicbest=BIC_min;
//      iret=list_cand(ind_stock_bool1(bettercand-1),0);//-1 a cause de la stationarité
//      jret=list_cand(ind_stock_bool1(bettercand-1),1);
//      Zopt_zi = newZ_zi;//initialisation à la stationarité
//      Zopt_zj = newZ_zj;
//      Zopt_si = newZ_si;
//      Zopt_sj = newZ_sj;
//      value=-1;
//      debut_colj=0;
//      ajout=false;
//      complexite=newZ_zi.size();
//      //debut modification zopt(i,j)=1-zopt(i,j)
//      if(jret>1)//si on est pas en premiere colonne
//      {
//        for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
//        {
//          debut_colj=debut_colj+Zopt_sj(k);
//        }
//      }
//      place=debut_colj;
//      
//      if(Zopt_si(iret-1)!=0 && Zopt_sj(jret-1)!=0)
//      {
//        while(place<(debut_colj+Zopt_sj(jret-1)) )
//        {
//          if(Zopt_zi(place)==iret)
//          {
//            value=place;
//            place=complexite+10;//on sort de la boucle
//          }
//          else if (Zopt_zi(place)>iret)
//          {
//            place=complexite+10;//on sort de la boucle
//          }
//          else//on continue de chercher
//          {
//            place=place+1;
//          }
//        }
//      }
//      if(value==-1)//mode ajout
//      {
//        for(k=0;k<complexite;k++)
//        {
//          if(ajout==false && (Zopt_zj(k)>jret ||(Zopt_zj(k)==jret && Zopt_zi(k)>iret) ))//on ajoute
//          {            
//            zj_loc<<zj_loc,jret;//on passe par zj_loc
//            zi_loc<<zi_loc,iret;
//            ajout=true;
//          }
//          zj_loc<<zj_loc,Zopt_zj(k);
//          zi_loc<<zi_loc,Zopt_zi(k);
//        }
//        Zopt_zi=zi_loc;
//        Zopt_zj=zj_loc;
//        Zopt_si(iret-1)=Zopt_si(iret-1)+1;
//        Zopt_sj(jret-1)=Zopt_sj(jret-1)+1;
//        complexite=complexite+1;
//        if(ajout==false)//derniere position
//        {
//          Zopt_zj<<Zopt_zj,jret;
//          Zopt_zi<<Zopt_zi,iret;
//        }        
//      }
//      else//suppression
//      {
//        for(k=0;k<complexite;k++)
//        {
//          if(k!=value)
//          {
//            zj_loc(k)=Zopt_zj(k);
//            zi_loc(k)=Zopt_zi(k);
//          }
//        }
//        Zopt_zi=zi_loc;
//        Zopt_zj=zj_loc;
//        Zopt_si(i_loc-1)=Zopt_si(iret-1)-1;
//        Zopt_sj(jret-1)=Zopt_sj(jret-1)-1;
//        complexite=complexite-1;
//      }
//      //fin de la modifidation zopt(i,j)=1-zopt(i,j)
//      
//      if (ibla[0]>0)//commentaires
//      {
//        cout<<step<<" Bicbest: "<<Bicbest<<" complexite: "<<complexite<<"\n";
//        if (ibla[0]==2)
//        {
//          cout<<"nb_cand "<<nb_cand_retenu<<"BIC min "<<BIC_min<<"\n";
//        }
//      }
//      
//      if(ibest[0]==1)//on a vu le meilleur absolu donc si on veut y aller, on y va
//      { 
//        bicvect=BICZsparse_j_cpp(matX,Zopt_zi,Zopt_sj,Bic_vide_vect,bicvect,imet_BIC[0],n,jret);
//        newZ_zi=Zopt_zi;
//        newZ_zj=Zopt_zj;
//        newZ_si=Zopt_si;
//        newZ_sj=Zopt_sj;
//        sumbic=Bicbest;
//      }
//      else //tirage au sort
//      {
//        bicweight=stockBIC1;
//        for (int v=0;v<nb_cand_retenu;v++)
//        {
//          bicweight(v)=bicweight(v)-BIC_min;
//          bicweight(v)=exp(-bicweight(v)/2);
//          sumexp=sumexp+bicweight(v);
//        }
//        w=-1;//initialisation candidat choisi (il est a -1 car on va rajouter un "+1" avec le while)
//        ran=rand();
//        ran=ran/32767;
//        sumcum=0;
//        while (ran>=sumcum)//choix du candidat
//        { 
//          w++;
//          bicweight(w)=bicweight(w)/sumexp;
//          sumcum=sumcum+bicweight(w);
//        }
//        if(w==0)
//        {
//  //       cout<<"on reste au meme endroit";
//         station=true;
//        }
//        else
//        {
//          iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
//          jret=list_cand(ind_stock_bool1(w-1),1);
//          Zcand_zi=newZ_zi;
//          Zcand_zj=newZ_zj;
//          Zcand_si=newZ_si;
//          Zcand_sj=newZ_sj;
//          value=-1;
//          debut_colj=0;
//          ajout=false;
//          complexite=newZ_zi.size();
//          //debut de la modification Zcand(i,j)=1-Zcand(i,j)
//          if(jret>1)//si on est pas en premiere colonne
//          {
//            for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
//            {
//              debut_colj=debut_colj+Zcand_sj(k);
//            }
//          }
//          place=debut_colj;
//          
//          if(Zcand_si(iret-1)!=0 && Zcand_sj(jret-1)!=0)
//          {
//            while(place<(debut_colj+Zcand_sj(jret-1)) )
//            {
//              if(Zcand_zi(place)==iret)
//              {
//                value=place;
//                place=complexite+10;//on sort de la boucle
//              }
//              else if (Zcand_zi(place)>iret)
//              {
//                place=complexite+10;//on sort de la boucle
//              }
//              else//on continue de chercher
//              {
//                place=place+1;
//              }
//            }
//          }
//          if(value==-1)//mode ajout
//          {
//            for(k=0;k<complexite;k++)
//            {
//              if(ajout==false && (Zcand_zj(k)>jret ||(Zcand_zj(k)==jret && Zcand_zi(k)>iret) ))//on ajoute
//              {            
//                zj_loc<<zj_loc,jret;//on passe par zj_loc
//                zi_loc<<zi_loc,iret;
//                ajout=true;
//              }
//              zj_loc<<zj_loc,Zcand_zj(k);
//              zi_loc<<zi_loc,Zcand_zi(k);
//            }
//            Zcand_zi=zi_loc;
//            Zcand_zj=zj_loc;
//            Zcand_si(iret-1)=Zcand_si(iret-1)+1;
//            Zcand_sj(jret-1)=Zcand_sj(jret-1)+1;
//            complexite=complexite+1;
//            if(ajout==false)//derniere position
//            {
//              Zcand_zj<<Zcand_zj,jret;
//              Zcand_zi<<Zcand_zi,iret;
//            }
//          }
//          else//suppression
//          {
//            for(k=0;k<complexite;k++)
//            {
//              if(k!=value)
//              {
//                zj_loc(k)=Zcand_zj(k);
//                zi_loc(k)=Zcand_zi(k);
//              }
//            }
//            Zcand_zi=zi_loc;
//            Zcand_zj=zj_loc;
//            Zcand_si(iret-1)=Zcand_si(iret-1)-1;
//            Zcand_sj(jret-1)=Zcand_sj(jret-1)-1;
//            complexite=complexite-1;
//          }
//          bicvect=BICZsparse_j_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],n,jret);
//          newZ_zi=Zcand_zi;
//          newZ_zj=Zcand_zj;
//          newZ_si=Zcand_si;
//          newZ_sj=Zcand_sj;
//          sumbic=stockBIC1(w);
//        }
//      }
//    } 
//    else if(nb_cand_retenu==1) //si la stationarité est le seul candidat
//    {
////      cout<<"on reste au meme endroit";
//      station=true;
//    }
//    else if(ibetter[0]==1 && bettercand!=0)//si on a un meilleur local et qu'on veut y aller, on y va
//    {
//      iret=list_cand(ind_stock_bool1(bettercand-1),0);//-1 a cause de la stationarité
//      jret=list_cand(ind_stock_bool1(bettercand-1),1);
//      Zcand_zi=newZ_zi;
//      Zcand_zj=newZ_zj;
//      Zcand_si=newZ_si;
//      Zcand_sj=newZ_sj;
//      value=-1;
//      debut_colj=0;
//      ajout=false;
//      complexite=newZ_zi.size();
//      //debut de la modification Zcand(i,j)=1-Zcand(i,j)
//      if(jret>1)//si on est pas en premiere colonne
//      {
//        for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
//        {
//          debut_colj=debut_colj+Zcand_sj(k);
//        }
//      }
//      place=debut_colj;
//          
//      if(Zcand_si(iret-1)!=0 && Zcand_sj(jret-1)!=0)
//      {
//        while(place<(debut_colj+Zcand_sj(jret-1)) )
//        {
//          if(Zcand_zi(place)==iret)
//          {
//            value=place;
//            place=complexite+10;//on sort de la boucle
//          }
//          else if (Zcand_zi(place)>iret)
//          {
//            place=complexite+10;//on sort de la boucle
//          }
//          else//on continue de chercher
//          {
//            place=place+1;
//          }
//        }
//      }
//      if(value==-1)//mode ajout
//      {
//        for(k=0;k<complexite;k++)
//        {
//          if(ajout==false && (Zcand_zj(k)>jret ||(Zcand_zj(k)==jret && Zcand_zi(k)>iret) ))//on ajoute
//          {            
//            zj_loc<<zj_loc,jret;//on passe par zj_loc
//            zi_loc<<zi_loc,iret;
//            ajout=true;
//          }
//          zj_loc<<zj_loc,Zcand_zj(k);
//          zi_loc<<zi_loc,Zcand_zi(k);
//        }
//        Zcand_zi=zi_loc;
//        Zcand_zj=zj_loc;
//        Zcand_si(iret-1)=Zcand_si(iret-1)+1;
//        Zcand_sj(jret-1)=Zcand_sj(jret-1)+1;
//        complexite=complexite+1;
//        if(ajout==false)//derniere position
//        {
//          Zcand_zj<<Zcand_zj,jret;
//          Zcand_zi<<Zcand_zi,iret;
//        }
//      }
//      else//suppression
//      {
//        for(k=0;k<complexite;k++)
//        {
//          if(k!=value)
//          {
//            zj_loc(k)=Zcand_zj(k);
//            zi_loc(k)=Zcand_zi(k);
//          }
//        }
//        Zcand_zi=zi_loc;
//        Zcand_zj=zj_loc;
//        Zcand_si(iret-1)=Zcand_si(iret-1)-1;
//        Zcand_sj(jret-1)=Zcand_sj(jret-1)-1;
//        complexite=complexite-1;
//      }
//      bicvect=BICZsparse_j_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],n,jret);
//      newZ_zi=Zcand_zi;
//      newZ_zj=Zcand_zj;
//      newZ_si=Zcand_si;
//      newZ_sj=Zcand_sj;
//      sumbic=BIC_min;
//    }
//    else if (irandom[0]==0)
//    {
//      station=true;
//    }
//    else//tirage au sort (on a pas de saut systématique)
//    {
//      bicweight=stockBIC1;
//      for (v=0;v<nb_cand_retenu;v++)
//      {
//        bicweight(v)=bicweight(v)-BIC_min;
//        bicweight(v)=exp(-bicweight(v)/2);
//        sumexp=sumexp+bicweight(v);
//      }
//      
//      w=-1;//initialisation candidat choisi (il est a -1 car on va rajouter un "+1" avec le while)
//      ran=rand();
//      ran=ran/32767;
//      sumcum=0;
//      while (ran>=sumcum)//choix du candidat
//      { 
//        w++;
//        bicweight(w)=bicweight(w)/sumexp;
//        sumcum=sumcum+bicweight(w);
//      }
//      if(w==0)
//      {
////       cout<<"on reste au meme endroit";
//       station=true;
//      }
//      else
//      {
//        iret=list_cand(ind_stock_bool1(w-1),0);//-1 car stationarité en premier dans bicweight
//        jret=list_cand(ind_stock_bool1(w-1),1);
//        Zcand_zi=newZ_zi;
//        Zcand_zj=newZ_zj;
//        Zcand_si=newZ_si;
//        Zcand_sj=newZ_sj;
//        value=-1;
//        debut_colj=0;
//        ajout=false;
//        complexite=newZ_zi.size();
//        //debut de la modification Zcand(i,j)=1-Zcand(i,j)
//        if(jret>1)//si on est pas en premiere colonne
//        {
//          for(k=0;k<jret-1;k++)//on regarde ou commence la colonne j
//          {
//            debut_colj=debut_colj+Zcand_sj(k);
//          }
//        }
//        place=debut_colj;
//        
//        if(Zcand_si(iret-1)!=0 && Zcand_sj(jret-1)!=0)
//        {
//          while(place<(debut_colj+Zcand_sj(jret-1)) )
//          {
//            if(Zcand_zi(place)==iret)
//            {
//              value=place;
//              place=complexite+10;//on sort de la boucle
//            }
//            else if (Zcand_zi(place)>iret)
//            {
//              place=complexite+10;//on sort de la boucle
//            }
//            else//on continue de chercher
//            {
//              place=place+1;
//            }
//          }
//        }
//        if(value==-1)//mode ajout
//        {
//          if(Zcand_si(jret-1)!=0 && Zcand_sj(iret-1)!=0)//pas de croisement
//          {
//            for(k=0;k<complexite;k++)
//            {
//              if(ajout==false && (Zcand_zj(k)>jret ||(Zcand_zj(k)==jret && Zcand_zi(k)>iret) ))//on ajoute
//              {            
//                zj_loc<<zj_loc,jret;//on passe par zj_loc
//                zi_loc<<zi_loc,iret;
//                ajout=true;
//              }
//              zj_loc<<zj_loc,Zcand_zj(k);
//              zi_loc<<zi_loc,Zcand_zi(k);
//            }
//            Zcand_zi=zi_loc;
//            Zcand_zj=zj_loc;
//            Zcand_si(iret-1)=Zcand_si(iret-1)+1;
//            Zcand_sj(jret-1)=Zcand_sj(jret-1)+1;
//            complexite=complexite+1;
//            if(ajout==false)//derniere position
//            {
//              Zcand_zj<<Zcand_zj,jret;
//              Zcand_zi<<Zcand_zi,iret;
//            }
//          }
//          else //croisement
//          {
//            for(k=0;k<complexite;k++)
//            {
//              if(ajout==false && (Zcand_zj(k)>jret ||(Zcand_zj(k)==jret && Zcand_zi(k)>iret) ))//on ajoute
//              {            
//                zj_loc<<zj_loc,jret;//on passe par zj_loc
//                zi_loc<<zi_loc,iret;
//                ajout=true;
//              }
//              if(Zcand_zj(k)!=iret&&Zcand_zi(k)!=jret)//si on doit pas supprimer, on recopie
//              {
//                zj_loc<<zj_loc,Zcand_zj(k);
//                zi_loc<<zi_loc,Zcand_zi(k);
//              }
//              else
//              {
//                Zcand_sj(Zcand_zj(k)-1)=Zcand_sj(Zcand_zj(k)-1)-1;
//                Zcand_si(Zcand_zi(k)-1)=Zcand_si(Zcand_zi(k)-1)-1;
//                complexite=complexite-1;
//              }
//            }
//            Zcand_zi=zi_loc;
//            Zcand_zj=zj_loc;
//            Zcand_si(iret-1)=Zcand_si(iret-1)+1;
//            Zcand_sj(jret-1)=Zcand_sj(jret-1)+1;
//            complexite=complexite+1;
//            if(ajout==false)//derniere position
//            {
//              Zcand_zj<<Zcand_zj,jret;
//              Zcand_zi<<Zcand_zi,iret;
//            }
//          }
//        }
//        else//suppression
//        {
//          for(k=0;k<complexite;k++)
//          {
//            if(k!=value)
//            {
//              zj_loc(k)=Zcand_zj(k);
//              zi_loc(k)=Zcand_zi(k);
//            }
//          }
//          Zcand_zi=zi_loc;
//          Zcand_zj=zj_loc;
//          Zcand_si(iret-1)=Zcand_si(iret-1)-1;
//          Zcand_sj(jret-1)=Zcand_sj(jret-1)-1;
//          complexite=complexite-1;
//        }
//        bicvect=BICZsparse_j_cpp(matX,Zcand_zi,Zcand_sj,Bic_vide_vect,bicvect,imet_BIC[0],n,jret);
//        newZ_zi=Zcand_zi;
//        newZ_zj=Zcand_zj;
//        newZ_si=Zcand_si;
//        newZ_sj=Zcand_sj;
//        sumbic=stockBIC1(w);
//      }
//    }
//    
//    if(sumbic==Bicbest)//on regarde si le le BIC est le meme que le BIC optimal
//    {
//      nb_opt=nb_opt+1;//si c'est le cas alors on a retrouve une fois de plus BICbest
//      if(nb_opt==inb_opt_max[0])//si on a atteint le nombre maximum de fois ou on a retrouvé BICbest
//      {
//        step=imaxiter[0];//alors on sort du while
//        cout<<"convergence atteinte";
//      }
//    }
//    
//    
//    if(iplot[0]==1)//si on veut des graphiques on mets à jour les vecteurs
//    {
//      bic_etape (step)=sumbic;
//      complexite_etape (step)=complexite;
////      if(relax_effect==true)
////      {
////        type_changement_etape=3;
////      }
////      if (station==true)
////      {
////        etape (step)=2;//stationarité
////      }
////      else if(newZ(iret,jret)==0)
////      {
////        etape (step)=0;//suppression
////      }
////      else
////      {
////        etape (step)=1;//ajout
////      }
//    }
//    if(ibla[0]>2)//commentaires lors des etapes
//    {
//      cout<<step<<" BIC local: "<<sumbic<<" compléxité local: "<<complexite<<"\n";
//    }
//    step=step+1; 
//  }//fin des étapes (while)
//  
//  if(iplot[0]==1)
//  {
//    return List::create(
//      Named("newZ_zi")=  newZ_zi,
//      Named("newZ_zj")=  newZ_zj,
//      Named("newZ_si")=  newZ_si,
//      Named("newZ_sj")=  newZ_sj,      
//      Named("bic_loc")=  sumbic,
//      Named("Zopt_zi")=  Zopt_zi,
//      Named("Zopt_zj")=  Zopt_zj,
//      Named("Zopt_si")=  Zopt_si,
//      Named("Zopt_sj")=  Zopt_sj,
//      
//      Named("bic_opt")=  Bicbest,
//      Named("bic_etape")=  bic_etape,
//      Named("complexite_etape")=  complexite_etape,
//      Named("etape")=  etape
//    );
//  }
//  else
//  {
//  return List::create(
//      Named("newZ_zi")=  newZ_zi,
//      Named("newZ_zj")=  newZ_zj,
//      Named("newZ_si")=  newZ_si,
//      Named("newZ_sj")=  newZ_sj, 
//      Named("bic_loc")=  sumbic,
//      Named("Zopt_zi")=  Zopt_zi,
//      Named("Zopt_zj")=  Zopt_zj,
//      Named("Zopt_si")=  Zopt_si,
//      Named("Zopt_sj")=  Zopt_sj,
//      Named("bic_opt")=  Bicbest
//    );
//  }
//  END_RCPP
//}
