#!/usr/bin/env Rscript

COLORS <- c("green", "red", "blue", "yellow", "orange", "pink", "brown")

CLT <- function(n) {
    # Scrieți o funcție care genereaza un număr de n eșantioane,
    # fiecare eșantion de dimensiune 10000, folosind distribuția
    # uniforma cu valori din intervalul [−10, 10].
    # Numărul de eșantioane n este parametrul funcției.
    # Funcția returneaza vectorul mediilor celor n eșantioane.
    medii <- c()
    for (index in 1:10000) {
        esantion <- runif(n=n, min=-10, max=10)
        medii[index] <- mean(esantion)
    }
    return(medii)
}

histograma <- function(image_name, n) {
    # Genereaza histograma pentru datele primite
    png(image_name, width=4, height=4, units="in", res=300)
    hist(CLT(n), col=COLORS, nclass=100)
    dev.off()
}

main <- function() {
    # Folosiți aceasta funcție pentru a construi histograma mediilor
    # eșantioanelor când numărul acestora n este egal cu 1, 5, 10 și
    # 100, respectiv.
    # Observați modificarile histogramei când valorile lui n cresc.
    for (valoare in c(1, 5, 10, 100, 200, 500, 1000)) {
        histograma(paste('histograma-', valoare, '.png', seq=""), valoare)
    }
}

main()
