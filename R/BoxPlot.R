#' Boxplot with confidence interval and ANOVA on the plot.
#'@description classical boxplot function improved with integrated confidence level on the mean for each group ploted on the graph and also ANOVA with p-value and its interpretation given in the legend.
#' @param num a numeric vector to plot boxplot(num~grp). Represents the value that will be compared between the groups.
#' @param grp a qualitative vector (factor) to plot boxplot(num~grp). Represents the groups we will compare.
#' @param data a data.frame (or list) from which the variables in formula should be taken.
#' @param AnoVa boolean to compute or not anova (when multiple groups) to see if they differ in mean.If false the Kruskal-Wallis Rank Sum test is computed instead.
#' @param risk the risk value used for confidence intervals. 
#' @param lang lingustic parameter to specify the language of the legend
#' @param verbose boolean to make a test and print the result in the subtitle
#' @param ... Other graphical parameters 
#' @export
#' @examples

#' require(CorReg)
#' repart=c(20,40,40)
#' X=data.frame(num=c(rnorm(repart[1],10,1),rnorm(repart[2],11,1),rnorm(repart[3],10,1)),
#' grp=c(rep("A",times=repart[1]),rep("B",times=repart[2]),rep("C",times=repart[3])))
#' 
#' 
#' BoxPlot(X$num,X$grp,data=X,ylab="num",main="boxplot with confidence intervals")
#' #Confidence interval in red with mean in blue.
#' 
#' 
BoxPlot<-function(num,grp=NULL,data=NULL,AnoVa=TRUE,risk=0.05,lang=c("en","fr"),verbose=TRUE,...){
   
   if(!is.null(grp)){
      formula=num~grp
      if (AnoVa){#if we want anova : we compute it and stock the p-value in the subtitle
         resanov=aov(formula,data=data)
         pval=summary(resanov)[[1]][["Pr(>F)"]][[1]]
      }else{  #suspicion of non-normalityand/or heteroscedasticity so kruskal-Wallis instead.
         grp=as.factor(grp)#characters do not work
         pval=kruskal.test(num,grp,data=data)$p.value
      }
      phrase=""
      if (pval<risk){
         if(lang[1]=="fr"){
            phrase="Les groupes semblent distincts en moyenne."
         }else{
            phrase="Groups seem to differ in mean"
         }
      }else{
         if(lang[1]=="fr"){
            phrase="Les groupes ne semblent pas distincts en moyenne"
         }else{
            phrase="groups don't seem to differ in mean"
         }
      }
      test=ifelse(AnoVa,"(AnoVa)","(Kruskal-Wallis)")
      if(verbose){sub=paste("p-value=",round(pval,4), " ",phrase, test )}
      else{sub=""}
   }else{#1 seule classe : grp=NULL
      sub=""
      formula=num
      pval=NA
   }
   
   res=boxplot(formula,sub=sub,...) 
   
   res$pval=pval
   

   y0=c()
   y1=c()
   y2=c()
   if(is.null(grp)){
      grp=1
      groupes=1
      x0=1
   }else{
      groupes=sort(unique(grp))
      
      x0 = seq(from = 1, length.out = length(groupes), by = 1) 
      
   }
   for (k in groupes){#k n'est pas forcement un indice, c'est le nom du groupe en cours
      Xloc=num[grp==k]
      y0 = c(y0,mean(Xloc)-qnorm(1-risk/2)*sd(Xloc)/sqrt(length(Xloc)))
      y1 = c(y1,mean(Xloc)+qnorm(1-risk/2)*sd(Xloc)/sqrt(length(Xloc)))
      y2=c(y2,mean(Xloc))
   }
   arrows(x0 = x0, y0 = y0, x1 = x0, y1 = y2, angle = 90, code = 3, lwd = 2,col="blue")
   arrows(x0 = x0, y0 = y0, x1 = x0, y1 = y1, angle = 90, code = 3, lwd = 2,col="red")
   res$confint=rbind(y1,y2,y0)
   row.names(res$confint)<-c("upper","mean","lower")
   res$risk=risk

   return(res)
} 