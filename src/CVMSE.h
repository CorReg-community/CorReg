#ifndef _CorReg_CVMSE_H
#define _CorReg_CVMSE_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP CVMSE(SEXP RX,SEXP RY,SEXP RK, SEXP Rintercept,SEXP Rmethode, SEXP Rgroupe) ;

#endif
