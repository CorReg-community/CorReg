#ifndef _CorReg_modsparse_cpp_H
#define _CorReg_modsparse_cpp_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP modsparse_cpp(SEXP Zi,SEXP Zj,SEXP Si,SEXP Sj,SEXP complexite,SEXP p,SEXP i,SEXP j,SEXP relax,SEXP rmax) ;

#endif
