#!/usr/bin/env Rscript

expression <- function(x) {
    y = 2^x
}

draw_plot <- function(start, stop) {
	# Funcția afisează graficul funcției `f(x) = 2x` pe un interval 
	# specificat [a, b].
	# Se va utiliza culoarea roșie pentru grafic cu grosimea liniei 2.
	# Graficul va trebui sa aibă ca titlul 'f(x)=2ˆx, x in [a,b]'.
    title <- paste("f(x)=2^x, x in [", start, ", ", stop, "]", seq="")
	curve(expression, start, stop, col="red", lwd=2, main=title)
}

draw_plot(1, 7)
