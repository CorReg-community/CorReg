#ifndef _CorReg_BicLoc_cpp_H
#define _CorReg_BicLoc_cpp_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport double BicLoc_cpp(const Eigen::MatrixXd &X,const Eigen::MatrixXd &Y,const bool &intercept,const int &methode) ;

#endif
