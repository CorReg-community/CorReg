#' Histograms with clusters
#' @param x a vector, matrix or data.frame on which the histogram will be computed and ploted.
#' @param plot if FALSE, the histogram is only computed with no graphical output.
#' @param classes vector of classes to color
#' @param col numeric color
#' @param mode one of c("classical","cumsum","density")
#' @param breaks by default : "Sturges"
#' @param ... 	further arguments and graphical parameters passed to plot.histogram and thence to title and axis.
#' @examples
#' require(CorReg)
#' x<-c(rnorm(50,0,1),rnorm(50,1,1))
#' classes<-rep(1:2,each=50)
#' Hist(x,classes)
#' @export
Hist<-function(x,classes=NULL,plot=TRUE,col=2:10,mode=c("classical","cumsum","density"),breaks="Sturges",...){
  x=as.numeric(x)
  mode<-match.arg(mode)
  if(is.null(classes)){#classic histogram
    res=hist(x,plot=plot,breaks=breaks,...)
  }else{#C'est parti
    if(breaks[1]=="Sturges"){
      xrange=hist(x,plot=FALSE)$breaks
    }else{
      xrange=breaks
      xrange=c(min(x),xrange,max(x))
      #TODO : n'ajouter les extremes que si necessaire
    }
    list_classes=unique(classes)
    hvtots=c()
    for (qui in list_classes){
      # 2) Have your vectors ready
      vi=x[classes==qui]
      # 3) subset your vectors to be inside xrange
      vi = subset(vi,vi<=max(xrange) & vi>=min(xrange))
      # 4) Now, use hist to compute the counts per interval
      hvi = hist(vi,breaks=xrange,plot=FALSE)$counts
      #hv3 = hist(v3,breaks=xrange,plot=FALSE)$counts
      hvtots=rbind(hvtots,hvi)
    } 



    # 5) Finally, Generate a Frequency BarPlot that is equivalent to a Stacked histogram
    #barplot(rbind(hv1,hv2,hv3),col=2:4,names.arg=xrange[-1],space=0,las=1,main=maintitle)
    if(breaks[1]!="Sturges"){
      xrange[1]=paste("<", xrange[2],sep="")
    npoints=length(xrange)
    xrange[npoints]=paste(">", xrange[npoints-1],sep="")#TODO un des deux est une inegalite large + indiquer cela dans la doc de la fonction
    }
    if(mode=="density"){

      # 6) You can also generate a Density Barplot
      Total = colSums(hvtots) #+ hv3
      #barplot(rbind(hv1/Total,hv2/Total,hv3/Total),col=2:4,names.arg=xrange[-1],space=0,las=1)
      barplot(hvtots/Total,names.arg=xrange[-1],space=0,las=1,col=col,...)
      #TODO : ajouter la ligne grise comme dans FlashProcess
      res=list(counts=hvtots/Total)
    }else if (mode=="cumsum"){
      barplot(t(apply(hvtots,1,cumsum)),names.arg=xrange[-1],space=0,las=1,col=col,...)
      res=list(counts=hvtots)

    }else{
      barplot(hvtots,names.arg=xrange[-1],space=0,las=1,col=col,...)
      res=list(counts=hvtots)
    }

  }

  return(res)
}
