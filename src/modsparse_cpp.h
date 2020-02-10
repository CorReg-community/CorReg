#ifndef _CorReg_OLS_H
#define _CorReg_OLS_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP modsparse_cpp(SEXP Zi,SEXP Zj,SEXP Si,SEXP Sj,SEXP complexite,SEXP p,SEXP i,SEXP j,SEXP relax,SEXP rmax) ;

#endif
