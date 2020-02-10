#ifndef _CorReg_OLS_H
#define _CorReg_OLS_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP OLS(SEXP X,SEXP Y,SEXP intercept,SEXP sigma,SEXP Bic,SEXP methode) ;

#endif
