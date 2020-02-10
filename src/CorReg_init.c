#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP BicLoc_cpp(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP BicTheta(SEXP, SEXP, SEXP, SEXP);
extern SEXP BicZ(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP BICZsparse(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP cleancolZ(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP cleanZ(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP CVMSE(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP GM_likelihood(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP modsparse_cpp(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP OLS(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rechercheZ_rejet(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rechercheZ_relax(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rechercheZ_sparse_relax(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
   {"BicLoc_cpp",              (DL_FUNC) &BicLoc_cpp,               6},
   {"BicTheta",                (DL_FUNC) &BicTheta,                 4},
   {"BicZ",                    (DL_FUNC) &BicZ,                     6},
   {"BICZsparse",              (DL_FUNC) &BICZsparse,               8},
   {"cleancolZ",               (DL_FUNC) &cleancolZ,                7},
   {"cleanZ",                  (DL_FUNC) &cleanZ,                   7},
   {"CVMSE",                   (DL_FUNC) &CVMSE,                    6},
   {"GM_likelihood",           (DL_FUNC) &GM_likelihood,            9},
   {"modsparse_cpp",           (DL_FUNC) &modsparse_cpp,           10},
   {"OLS",                     (DL_FUNC) &OLS,                      6},
   {"rechercheZ_rejet",        (DL_FUNC) &rechercheZ_rejet,        16},
   {"rechercheZ_relax",        (DL_FUNC) &rechercheZ_relax,        16},
   {"rechercheZ_sparse_relax", (DL_FUNC) &rechercheZ_sparse_relax, 16},
   {NULL, NULL, 0}
};

void R_init_CorReg(DllInfo *dll)
{
   R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
   R_useDynamicSymbols(dll, FALSE);
}
