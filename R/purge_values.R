#' Replaces unwanted values by NAs
#' @description Find values in a dataframe and replace them by NAs. Also give the liste of the variables implied
#' Beware of the factors. The variables stays as factors and the level is still in memory.
#' @param base the dataframe to clean
#' @param value the value or vector of value to find and remove. if "space" it removes the blank thousands separator.
#' @return a list with "base" as the cleaned dataset and "list_var" as the list of indexes of variables that have been changed.
#' @export

purge_values<-function(base,value){
  list_var=c()
  if(value[1]=="space"){#Asci 160
    chr <- function(n) { rawToChar(as.raw(n)) }
    asc <- function(x) { strtoi(charToRaw(x),16L) }
    value=chr(160)
    for (i in 1:ncol(base)){
      base[,i]=gsub(x = base[,i],pattern = chr(160),replacement = "")
    }
  }
  for(i in 1:ncol(base)){
    vecloc=which(base[,i]%in%value)
    base[vecloc,i]=NA
    if(length(vecloc)>0){
      list_var=c(list_var,i)
    }
  }
  return(list(base=base,list_var=list_var))
}