#' To clean non numeric values in a vector
#' @description Replace all non numeric values in a vector by NA's and change the vector format to be numeric
#' @param x The vector to clean
#' @export

Numeric_Only<-function(x=NULL){
  x[is.na(Is.Numeric(x))]=NA
  return(as.numeric(as.character(x)))
}