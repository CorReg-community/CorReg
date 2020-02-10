#ifndef _CorReg_kronecker_sum_H
#define _CorReg_kronecker_sum_H



#include <RcppEigen.h>
using namespace Rcpp ;
Rcpp::NumericVector kronecker_sum_cpp(const Rcpp::NumericVector &V1,const Rcpp::NumericVector &V2);

#endif
