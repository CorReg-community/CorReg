#ifndef _CorReg_BICZsparse_H
#define _CorReg_BICZsparse_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP BICZsparse(SEXP X,SEXP Z_Zi,SEXP Z_Sj,SEXP Bic_vide_vect,SEXP BicOld,SEXP methode,SEXP Zold_Sj,SEXP nrow) ;

#endif
