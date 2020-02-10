# ' Bic of one regression
# ' 

BicLoc<-function(X=X,Y=Y,intercept=TRUE,sigma=TRUE,SumSquare=FALSE,methode=1){
  ret= .Call( "BicLoc_cpp", X,Y,intercept,sigma,SumSquare,methode, PACKAGE = "CorReg")
  return(ret)
}