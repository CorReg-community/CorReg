#' Boxplot with confidence interval and ANOVA on the plot.
#' 
#' @description pltoly/ggplot2 boxplot function improved with integrated confidence level on the mean for each group ploted on the graph and also ANOVA with p-value and its interpretation given in the legend.
#' 
#' @param var a numeric vector to plot boxplot. Represents the value that will be compared between the groups.
#' @param responsive_var a qualitative vector (factor) to plot boxplot. Represents the groups we will compare.
#' @param threshold_line numeric value for threshold (equivalent of abline).
#' @param main title of the boxplot.
#' @param engine to choose ggplot2 enhanced by plotly (default) or only plotly. 
#' 
#' @examples
#' repart = c(20, 40, 40)
#' X = data.frame(num = c(rnorm(repart[1], 10, 1), rnorm(repart[2], 11, 1), rnorm(repart[3], 10, 1)),
#'                grp = rep(c("A", "B", "C"), repart))
#' 
#' 
#' BoxPlotly(X$num, X$grp, threshold_line = 12.7, main = "boxplot with confidence intervals")
#' # Confidence interval in red with mean in blue.
#' 
#' @export
BoxPlotly<-function(var, responsive_var=NULL, threshold_line=NULL, main=NULL, 
                    engine="ggplot2"){
  
  # Initialization of parameters for the script
  if(is.null(main)){
    main = paste("Boxplot of",deparse(substitute(var)))
  }
  
  if(is.null(responsive_var)){
    xname = ""
    
    #1 seule classe : responsive_var=NULL
    sub=""
    formula=var
    pval=NA
  }else{
    xname = deparse(substitute(responsive_var))
    
    # FROM CORREG : à refaire
    AnoVa=TRUE
    risk=0.05
    lang=c('fr','en')
    verbose=TRUE
    formula=var~responsive_var
    data=data.frame(var,responsive_var)
    if(AnoVa){#if we want anova : we compute it and stock the p-value in the subtitle
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
  }
  
  yname = deparse(substitute(var))
  
  if(engine=="ggplot2"){
    data = data.frame(var, responsive_var)
    p = ggplot(data, aes(x=responsive_var, y=var, fill=responsive_var)) +
      theme(plot.title = element_text(hjust = 0.5)) +
      geom_boxplot(notch = TRUE)
    
    p = p + stat_summary(fun.data = mean_se, geom = "errorbar", colour="red")
    p = p + stat_summary(fun.y = mean, geom = "point", colour="blue")
    p = p + labs(x=xname, y=yname)
    p = p + theme(legend.title = element_blank())
    
    
    if(!is.null(threshold_line)){
      p = p + geom_hline(yintercept = threshold_line, color = "red")
    }
    
    p = ggplotly(p)
    p = layout(p, title = list(text = paste0(main,'<br>','<sup>',sub,'</sup>')))
    
    p
  }else if(engine=="basic_plotly"){
    # Initialization of the graph
    p <- plot_ly()
    
    if(!is.null(responsive_var)){
      data = data.frame(var, responsive_var)
      
      # Determination of response values, ordered by freq asc.
      responsive_var_values = unique(responsive_var)
      responsive_var_values = responsive_var_values[order(responsive_var_values)]
      
      # Adding the different trace representing a factor's level
      for(responsive_value in responsive_var_values){
        p = add_trace(p, type = "box", 
                      y = data[data$responsive_var==responsive_value,]$var,
                      name = responsive_value)
      }
    }else{
      p = add_trace(p, type = "box", y = var, name = "")
    }
    
    p = layout(p, title = main, yaxis = list(title = yname),
               xaxis = list(title = xname), title = main)
    
    p
  }
}