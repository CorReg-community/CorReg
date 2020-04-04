#'Counts events
#' 
#' @description creates a data.frame counting occurencies of specific events
#' in a given vector separated by named connex groups.
#' For example it allows to count occurencies of each category of a qualitative
#'  variable above in the dataset for each individual. 
#'  Grouping allows to reset the count each time another variable changes.
#'  
#' The idea is to create a cumulative history of what happened before 
#' current individual.
#' 
#' One new variable is created for each unique value of events
#' 
#' @param events The vector of events to count (qualitative values). 
#' One new variable is created for each unique value of events.
#' @param group qualitative vector that resets counting when its value changes.
#' @param base a dataframe to which counting can be added (cbind).
#' @param prefix (optional) prefix to the newly created variables names
#' @param suffix (optional) suffix to the newly created variables names
#' @param clean (boolean) to decide wether or not variables names has to be 
#' cleaned (spaces, accents and special characters)
#' @param keep (boolean) to decide wether or not events and group are to 
#' be added to the resulting data.frame
#' @return a data.frame wich number of rows is the length of events
#' 
#' @example 
#' require(CorReg)
#' events= c(rep(c("a","b","c"),times=20))
#' group= rep(c(rep("A",times=12),rep("B",times=8)),times=3)
#' counting_events_group(events=events,group=group)
#' @export

counting_events_group<-function(events=events,group=NULL,base=NULL,
                                prefix=NULL,suffix=NULL,clean=TRUE,keep=FALSE){
  # base=base
  # events=base$cat_aci
  # group=base$NO_PRG
  # table(base$cat_aci,base$fam_pro_vis)
  if(is.null(group)){
    group=rep(1,times=length(events))
  }
  families=unique(events)
  ##Initialisation
  additional=data.frame(cbind(group,events))#creates a data.frame same nb of rows
  
  #for each family :
  for(family in families){
    additional=cbind(additional,0)
    #create a vector (list element or matrix,...)
  }
  #rename them if necessary (using families list)
  names(additional)[-c(1,2)]=paste0(prefix,as.character(families),suffix)
  if(clean==TRUE){names(additional)=clean_symbols(names(additional))}
  
  #define a variable to store local Montage 
  #(inital value = first met after timestamp ordering)
  local_prog=group[1]
  
  ##Core code
  #For each coil
  for (i in 2:nrow(additional)){
    #if new montage=> all values to 0
    if(group[i]!=group[i-1]){
      #set local Montage value to actual
      local_prog=group[i]
    }else{
      #else (same montage) 
      #=> copy previous line
      additional[i,-c(1,2)]=additional[i-1,-c(1,2)]
      #add one to the element corresponding to previous value
      family_loc=which(families==events[i-1])#get previous family
      additional[i,family_loc+2]=additional[i,family_loc+2]+1
    }
  }
  if(keep==FALSE){#don't keep columns used for counting
    additional=additional[,-c(1,2)]
  }
  #add new elements to the dataset
  if(!is.null(base)){
    additional=cbind(base,additional)
  }
  return(additional)
}