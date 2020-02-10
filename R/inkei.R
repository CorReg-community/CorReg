inkei <- function(length=35, squirt=4, thickness=4)
{
   if (thickness <= 3){thickness=4}
   
   message("          ", rep("_", length), "\n", sep="")
   
   for (t in 1:floor(thickness/2))
   {     message("          ", rep(" ", length+t), "\\\n", sep="")}
   
   message("          ", rep(" ", length+floor(thickness/2)), "-o", rep("~", squirt), "\n", sep="")
   
   for (t in 1:(floor(thickness/2)-1))
   {
      message("          ", rep(" ", length+floor(thickness/2)-t+1), "/\n", sep="")
   }
   
   message("          ", rep("_", length+1), "/\n", sep="")
   message("          \\
             \\
               \\
       .... |...|
       .... |...|
       .... |...|
       .... |...|
     \\____/____/")
}
#copyright Jim Aloku