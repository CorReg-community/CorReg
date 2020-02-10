#ifndef _CorReg_BICZsparse_j_cpp_H
#define _CorReg_BICZsparse_j_cpp_H

#include <RcppEigen.h>
using namespace Rcpp ;
Eigen::VectorXd BICZsparse_j_cpp(Eigen::MatrixXd X,Eigen::VectorXd Z_Zi,Eigen::VectorXd Z_Sj,Eigen::VectorXd Bic_vide_vect,Eigen::VectorXd BicOld,int methode,int nrow,int j) ;

#endif
