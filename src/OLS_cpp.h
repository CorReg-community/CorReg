#ifndef _CorReg_OLS_cpp_H
#define _CorReg_OLS_cpp_H

#include <RcppEigen.h>
using namespace Rcpp ;
using namespace Eigen;

MatrixXd OLS_cpp(const MatrixXd &X, const MatrixXd &Y,const bool &intercept,const int &methode) ;

#endif
