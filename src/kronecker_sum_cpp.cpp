#include "kronecker_sum_cpp.h"

using namespace Rcpp ;
using namespace Eigen;
using namespace std;

//V1 and V2 are column vectors from matrices (sub matrices)
Rcpp::NumericVector kronecker_sum_cpp(const Rcpp::NumericVector &V1,const Rcpp::NumericVector &V2){
    int size1=V1.size();
    int size2=V2.size();
    int size3=size1*size2;   
    Rcpp::NumericVector V3(size3);
    int ou=0;
    for(int i=0;i<size1;i++){
      for (int j=0; j<size2;j++){
        V3(ou)=V1(i)+V2(j);
        ou++;
      }
    }
  return(V3);
}
