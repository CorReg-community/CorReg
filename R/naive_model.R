#' How would it be if we were naive ?
#' @description Describe the result of a naive binary discriminant model
#' @param proba_1 The ratio of 1 in the population (if nb_1 is NULL)
#' @param effectif The global effective of the population
#' @param nb_1 The number of 1 in the population.If not NULL proba_1 is not read.default=NULL
#' @export
naive_model<-function(proba_1=0.8,effectif=100,nb_1=NULL){
  if(!is.null(nb_1)){
    vrai=rep(0,times=effectif)
    vrai[1:nb_1]=1
    proba_1=nb_1/effectif
  }else{
    vrai=runif(n = effectif,min=0,max=1)
    vrai[vrai>proba_1]=0
    vrai[vrai!=0]=1
  }
  pred=runif(n = effectif,min=0,max=1)
  pred[pred>proba_1]=0
  pred[pred!=0]=1
  count=table(pred,vrai)
  percent=count/effectif
  balance=data.frame(percent[1,1]+percent[2,2],percent[1,2]+percent[2,2]);names(balance)=c("TRUE","FALSE")
  return(list(count=count,percent=percent,balance=balance))
}

