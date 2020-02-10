#ifndef _CorReg_GM_likelihood_H
#define _CorReg_GM_likelihood_H

#include <RcppEigen.h>
using namespace Rcpp ;
RcppExport SEXP GM_likelihood(SEXP RY,SEXP RX,SEXP RBeta,SEXP RSigma,SEXP RM,SEXP Rnbclustmixmod,SEXP Rdetailsmixmod,SEXP RLog,SEXP Rintercept);

#endif
