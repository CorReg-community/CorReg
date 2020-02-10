#ifndef _CorReg_OLS_H
#define _CorReg_OLS_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP rechercheZ(SEXP X,SEXP Z,SEXP bic_vide_vect,SEXP methode_tirage,SEXP methode_newZ,SEXP methode_BIC,SEXP Rmax,SEXP p2max,SEXP Maxiter,SEXP plot,SEXP best,SEXP better,SEXP random,SEXP bla,SEXP nb_opt_max) ;

#endif
