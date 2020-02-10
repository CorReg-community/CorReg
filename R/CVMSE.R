#' Cross validation
#' @export
#' @param X covariates matrix (double)
#' @param Y response variable
#' @param K number of classes
#' @param intercept (boolean) with or without an intercept
#' @param methode the methode used by OLS.
#' @param groupe a vector to define the groups used for cross-validation (to obtain a reproductible result)

CVMSE<-function(X=X,Y=Y,K=K,intercept=TRUE,methode=1,groupe=NULL){
  K=abs(K)
  K=min(K,nrow(X))
  Y=as.double(Y)
  if(is.null(groupe)){
    groupe=rep(0:(K-1),length.out=nrow(as.matrix(X)))
    groupe=sample(groupe)
  }
  res=.Call( "CVMSE",X,Y,K,intercept,methode,groupe, PACKAGE = "CorReg")
  return(res$MSE)
}