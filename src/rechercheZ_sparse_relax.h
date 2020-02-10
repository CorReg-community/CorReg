#ifndef _CorReg_OLS_H
 #define _CorReg_OLS_H
 
 #include <RcppEigen.h>
 using namespace Rcpp ;
 RcppExport SEXP rechercheZ_sparse_relax(SEXP X,SEXP Zi,SEXP Zj,SEXP Si,SEXP Sj,SEXP bic_vide_vect,SEXP methode_tirage,SEXP methode_BIC,SEXP Rmax,SEXP Maxiter,SEXP plot,SEXP best,SEXP better,SEXP random,SEXP bla,SEXP nb_opt_max) ;
 
 #endif
