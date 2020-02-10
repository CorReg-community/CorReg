Is.Numeric<-function(x){
  res=as.character(as.numeric(as.character(x)))==as.numeric(as.character(x))
  return(res)
}