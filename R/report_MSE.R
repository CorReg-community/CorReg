#' Quickly reports some MSE
#'@description Some MSE reporting to have an overview of the predictive quality of a model.
#' @param real a numeric vector that contains the true values
#' @param prediction a numeric vector that contains the predicted values.
#' @return a list containing the MSE values. 
#' RMSE is the root MSE (square root of the mean square error)
#' Relative is the mean of the relative errors (Root errors divided by the real values)
#' !standard_Deviation is the standard deviation of the root errors
#' @export

report_MSE <- function(real,prediction){
   RMSE=mean(sqrt((prediction-real)^2))
   RelRMSE=mean(sqrt((prediction-real)^2)/real)*100
   SDRSE=sd(sqrt((prediction-real)^2))
   MAE=mean(abs(prediction-real))#Mean Absolute Error
   relMAE=mean(abs(prediction-real)/real)*100#Mean Absolute Error
   return(list(RMSE=RMSE,RelRMSE=RelRMSE,MAE=MAE,relMAE=relMAE,Standart_Deviation=SDRSE))
}
