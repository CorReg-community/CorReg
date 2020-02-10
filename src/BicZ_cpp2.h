#ifndef _CorReg_BicZ_cpp2_H
#define _CorReg_BicZ_cpp2_H

#include <RcppEigen.h>
using namespace Rcpp ;
Eigen::VectorXd BicZ_cpp2(Eigen::MatrixXd X,Eigen::MatrixXd Z,Eigen::VectorXd Bic_vide_vect,int methode) ;

#endif
