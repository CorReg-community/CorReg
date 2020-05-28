#' Remove and replace accents and special characters
#' 
#' @description Remove and replace accents and special characters. 
#' Created to adapt names of dataframes coming from excel files with 
#' weird column names. Replacement is made by underscore symbol.
#' 
#' @param input The vector to clean
#' 
#' @return a vector without special symbols and accents.
#' 
#'   
#' @export
clean_symbols <- function(input) {
    output <- input
    output = gsub(x = output, pattern = "\t", replacement = "") # remove tabs cause file is comma separated
    output = gsub(x = output, pattern = " ", replacement = "_") # remove spaces
    
    output = gsub(x = output, pattern = "\u00e8", replacement = "e") # remove accents
    output = gsub(x = output, pattern = "\u00e9", replacement = "e") # remove accents
    output = gsub(x = output, pattern = "\u00ea", replacement = "e") # remove accents
    output = gsub(x = output, pattern = "\u00eb", replacement = "e") # remove accents
    output = gsub(x = output, pattern = "\u00c8", replacement = "E") # remove accents
    output = gsub(x = output, pattern = "\u00c9", replacement = "E") # remove accents
    output = gsub(x = output, pattern = "\u00ca", replacement = "E") # remove accents
    output = gsub(x = output, pattern = "\u00cb", replacement = "E") # remove accents
    
    output = gsub(x = output, pattern = "\u00f2", replacement = "o") # remove accents
    output = gsub(x = output, pattern = "\u00f3", replacement = "o") # remove accents
    output = gsub(x = output, pattern = "\u00f4", replacement = "o") # remove accents
    output = gsub(x = output, pattern = "\u00f5", replacement = "o") # remove accents
    output = gsub(x = output, pattern = "\u00f6", replacement = "o") # remove accents
    output = gsub(x = output, pattern = "\u00f8", replacement = "o") # remove accents
    output = gsub(x = output, pattern = "\u00d2", replacement = "O") # remove accents
    output = gsub(x = output, pattern = "\u00d3", replacement = "O") # remove accents
    output = gsub(x = output, pattern = "\u00d4", replacement = "O") # remove accents
    output = gsub(x = output, pattern = "\u00d5", replacement = "O") # remove accents
    output = gsub(x = output, pattern = "\u00d6", replacement = "O") # remove accents
    output = gsub(x = output, pattern = "\u00d8", replacement = "O") # remove accents
    
    output = gsub(x = output, pattern = "\u00e0", replacement = "a") # remove accents
    output = gsub(x = output, pattern = "\u00e1", replacement = "a") # remove accents
    output = gsub(x = output, pattern = "\u00e2", replacement = "a") # remove accents
    output = gsub(x = output, pattern = "\u00e3", replacement = "a") # remove accents
    output = gsub(x = output, pattern = "\u00e4", replacement = "a") # remove accents
    output = gsub(x = output, pattern = "\u00e5", replacement = "a") # remove accents
    output = gsub(x = output, pattern = "\u00c0", replacement = "A") # remove accents
    output = gsub(x = output, pattern = "\u00c1", replacement = "A") # remove accents
    output = gsub(x = output, pattern = "\u00c2", replacement = "A") # remove accents
    output = gsub(x = output, pattern = "\u00c3", replacement = "A") # remove accents
    output = gsub(x = output, pattern = "\u00c4", replacement = "A") # remove accents
    output = gsub(x = output, pattern = "\u00c5", replacement = "A") # remove accents
    
    output = gsub(x = output, pattern = "\u00f9", replacement = "u") # remove accents
    output = gsub(x = output, pattern = "\u00fa", replacement = "u") # remove accents
    output = gsub(x = output, pattern = "\u00fb", replacement = "u") # remove accents
    output = gsub(x = output, pattern = "\u00fc", replacement = "u") # remove accents
    output = gsub(x = output, pattern = "\u00d9", replacement = "U") # remove accents
    output = gsub(x = output, pattern = "\u00da", replacement = "U") # remove accents
    output = gsub(x = output, pattern = "\u00db", replacement = "U") # remove accents
    output = gsub(x = output, pattern = "\u00dc", replacement = "U") # remove accents
    
    output = gsub(x = output, pattern = "\u00ec", replacement = "i") # remove accents
    output = gsub(x = output, pattern = "\u00ed", replacement = "i") # remove accents
    output = gsub(x = output, pattern = "\u00ee", replacement = "i") # remove accents
    output = gsub(x = output, pattern = "\u00ef", replacement = "i") # remove accents
    output = gsub(x = output, pattern = "\u00cc", replacement = "I") # remove accents
    output = gsub(x = output, pattern = "\u00cd", replacement = "I") # remove accents
    output = gsub(x = output, pattern = "\u00ce", replacement = "I") # remove accents
    output = gsub(x = output, pattern = "\u00cf", replacement = "I") # remove accents
    
    output = gsub(x = output, pattern = "\u00e7", replacement = "c") # remove cedilla
    output = gsub(x = output, pattern = "\u00c7", replacement = "C") # remove cedilla
    
    output = gsub(x = output, pattern = "\\.", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\-", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\+", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\=", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = ">", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "<", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\u00b0", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\:", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\", replacement = "_", fixed = TRUE) # remove symbol
    output = gsub(x = output, pattern = "\\)", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\(", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\/", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\_\\_", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\_\\_", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\_\\_", replacement = "_") # remove symbol
    output = gsub(x = output, pattern = "\\_\\_", replacement = "_") # remove symbol
    
    return(output)
}
