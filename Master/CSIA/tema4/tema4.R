#!/usr/bin/env Rscript

get_lm_formula <- function(space, house_data) {
    y <- paste("as.matrix(house_data[", 1, "])", sep="")
    x <- ""
    for(j in 1:length(space)){
        if(j==1){   x <- paste("as.matrix(house_data[", space, "])", sep="")       } 
        else    {   x <- paste(x, "as.matrix(house_data[", space[j], "])", sep="") }
        if(j != length(space)) { x <- paste(x, "+", sep="") }
    }
    result <- paste(y, x[1], sep="~")
    return(result)
}


residual_sum_of_squares <- function(index, space, house_data, rss_matrix) {
    # The residual sum of squares (RSS), also known as the sum of squared
    # residuals (SSR) or the sum of squared errors of prediction (SSE),
    # is the sum of the squares of residuals (deviations of predicted from
    # actual empirical values of data).
    rss <- c()
    for(contor in 1:nrow(space)) {
        formula <- get_lm_formula(space[contor,])
        lm_formula = lm(as.formula(formula))
        rss[contor] = sum(lm_formula$residuals ^ 2)
    }

    rss_matrix[index, 1] = min(rss)
    i_min = which.min(rss)
    
    for (contor in 1:ncol(space) ) {
        rss_matrix[index, contor+1] = space[i_min,contor]
    }
    
    return(rss_matrix)
}


exercitiul_1 <- function() {
    # Sa se determine pentru fiecare dimensiune *p* de la 1 la *n* = 13
    # modelul de regresie avand *p* variabile și care optimizeaza,
    # *respectiv*, *Residual Sum of Squares* (RSS), R^2, R^2a și CP.
    house_data <- read.table(file="house.dat", header=TRUE)
    rss_matrix <- matrix(nrow=13, ncol=14)

    for (index in 1:13) {
        space <- t(combn(2:14, index))
        rss_matrix <- residual_sum_of_squares(index, space, house_data, rss_matrix)
    }
    rss_matrix[is.na(rss_matrix)] <- 0
    
    print(rss_matrix)
    
    pdf('rss_matrix.pdf')
        plot(rss_matrix[,1])
    dev.off()
}

exercitiul_1()
