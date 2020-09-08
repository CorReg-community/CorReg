#include "ProbaZ_cpp.h"
#include <iostream>
#include <string>

using namespace Rcpp ;
using namespace sugar ;
using namespace Eigen;
using namespace std;
using Eigen::Map;
using Eigen::MatrixXd;
using Rcpp::as;

double ProbaZ_cpp(Eigen::MatrixXd Z){
   int p=Z.cols();
   int p2=0;
   double logproba=0;
   Eigen::ArrayXXd SumCol(1,p);//SumCol est un vecteur qui contient la somme de chaque colonne de Z
   
   SumCol=Z.colwise().sum();//SumCol est un vecteur qui contient la somme de chaque colonne de Zcand
   
   for (int j = 0; j < p; j++){//pour chaque colonne de Z
        if(SumCol(j)>0){// si variable a gauche
            p2++;//on l'ajoute a p2
        }
   }
   if(p2>0){
      for (int j = 0; j < p; j++){//pour chaque colonne de Z
        if(SumCol(j)>0){// si variable a gauche
            logproba=logproba-log(double(R::choose((p-p2),SumCol(j))));
        }
      }
      logproba=logproba-log(double(p2))-double(p2)*log(double(p-p2))-log(double(R::choose(p,p2)));
   }  
   return logproba;
}
