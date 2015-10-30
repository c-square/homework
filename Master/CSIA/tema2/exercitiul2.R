#!/usr/bin/env Rscript

init <- function(start, stop) {
    # Pregătește setul de date
    array <- c()
    for(index in start:stop){
        array[length(array) + 1] = 2 ^ index
    }
    return(array)
}

generate_plot <- function(filename, n, p) {
    # Generează un fișier .PNG cu reprezentarea grafică a distribuției binomiale. 
    # Indicație: ?plot, ?title.
    x <- init(0, n)
    png(filename, width=4, height=4, units="in", res=300)
    plot(x, dbinom(x, n, p))
    dev.off()
}

grafice <- function(){
    # Generează graficele corespunzatoare distribuției binomiale `B(n, x, p)`,
    # pentru n = 20 și `p = 0.1, 0.2, . . . , 0.9`.
    for(index in seq(0.1, 0.9, 0.1)) {
        generate_plot(paste('dbinom-', index, '.png', seq=""), 20, index)
    }
}

grafice()
