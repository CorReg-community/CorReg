# CorReg

[![Travis build status](https://travis-ci.org/Quentin62/CorReg.svg?branch=master)](https://travis-ci.org/Quentin62/CorReg) [![AppVeyor build status](https://ci.appveyor.com/api/projects/status/github/Quentin62/CorReg?branch=master&svg=true)](https://ci.appveyor.com/project/Quentin62/CorReg)

[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/CorReg)](https://cran.r-project.org/package=CorReg) [![Total Downloads](http://cranlogs.r-pkg.org/badges/grand-total/CorReg?color=blue)](http://cranlogs.r-pkg.org/badges/grand-total/CorReg) [![Downloads](https://cranlogs.r-pkg.org/badges/CorReg)](https://cran.rstudio.com/web/packages/CorReg/index.html)

The code was originally on an [R-forge repository](https://r-forge.r-project.org/projects/correg/).

## CorReg's Concept

This package was motivated by correlation issues in real datasets, in particular industrial datasets. 

The main idea stands in explicit modeling of the correlations between covariates by a structure of sub-regressions (so it can model complex links, not only correlations between two variables), that simply is a system of linear regressions between the covariates. It points out redundant covariates that can be deleted in a pre-selection step to improve matrix conditioning without significant loss of information and with strong explicative potential because this pre-selection is explained by the structure of sub-regressions, itself easy to interpret. An algorithm to find the sub-regressions structure inherent to the dataset is provided, based on a full generative model and using Monte-Carlo Markov Chain (MCMC) method. This pre-treatment does not depend on a response variable and thus can be used in a more general way with any correlated datasets. 

In a second part, a plug-in estimator is defined to get back the redundant covariates sequentially. Then all the covariates are used but the sequential approach acts as a protection against correlations. 

This package also contains some functions to make statistics easier.

## Installation

```
library(devtools)
install_github("CorReg/CorReg", build_vignettes = TRUE)
```

## Vignette

Once the package is installed, a vignette showing an example is available using the R command:

```
RShowDoc("CorReg", package = "CorReg")
```

## Credits

**CorReg** is developed by Clément Théry with contributions from Christophe Biernacki, Gaétan Loridant, Florian Watrin and the A106 team: Quentin Grimonprez, Vincent Kubicki, Samuel Blanck, Jérémie Kellner.

Copyright ArcelorMittal

## Licence

[CeCILL](https://cecill.info/licences/Licence_CeCILL_V2.1-en.txt)

## References

Model-based covariable decorrelation in linear regression (CorReg): application to missing data and to steel industry. C Thery - 2015, http://www.theses.fr/2015LIL10060

