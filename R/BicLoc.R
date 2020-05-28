# ' Bic of one regression
# ' 

BicLoc <- function(X = X, Y = Y, intercept = TRUE, methode = 1) {
  ret = .Call("BicLoc_cpp", X, Y, intercept, methode, PACKAGE = "CorReg")
  return(ret)
}