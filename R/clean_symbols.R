#' Remove and replace accents and special characters
#' 
#' @description Remove and replace accents and special characters. 
#' Created to adapt names of dataframes coming from excel files with 
#' weird column names. Replacement is made by undersocre symbol.
#' 
#' @param input The vector to clean
#' 
#' @return a vector without special symbols and accents.
#' 
#'   
#' @export
clean_symbols<-function(input){
  output<-input
  output=gsub(x = output,pattern="\t",replacement = "")#remove tabs cause file is comma separated
  output=gsub(x = output,pattern=" ",replacement = "_")#remove spaces
  output=gsub(x = output,pattern="é",replacement = "e")#remove accents
  output=gsub(x = output,pattern="è",replacement = "e")#remove accents
  output=gsub(x = output,pattern="ê",replacement = "e")#remove accents
  output=gsub(x = output,pattern="à",replacement = "a")#remove accents
  output=gsub(x = output,pattern="ô",replacement = "o")#remove accents
  output=gsub(x = output,pattern="â",replacement = "a")#remove accents
  output=gsub(x = output,pattern="û",replacement = "u")#remove accents
  output=gsub(x = output,pattern="ù",replacement = "u")#remove accents
  output=gsub(x = output,pattern="Ø",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\.",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\-",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\+",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\=",replacement = "_")#remove symbol
  output=gsub(x = output,pattern=">",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="<",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\°",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\:",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\",replacement = "_",fixed=TRUE)#remove symbol
  output=gsub(x = output,pattern="\\)",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\(",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\/",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\_\\_",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\_\\_",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\_\\_",replacement = "_")#remove symbol
  output=gsub(x = output,pattern="\\_\\_",replacement = "_")#remove symbol
  return(output)
}