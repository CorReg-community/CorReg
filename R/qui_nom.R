# compare les noms presents dans deux vecteurs et les identifie
#
qui_nom <- function(nomcompl = nomcompl, nom = nom){
  loc = c(nom, nomcompl)
  return(which(duplicated(loc))-length(nom))
}
