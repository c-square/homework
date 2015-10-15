#!/usr/bin/env Rscript

# Citiți de la tastatură un vector conținănd 7 numere reale.
print("Introduceti 7 numere intregi:")
date <- scan(file("stdin"), what=integer(), nmax=7)

# Calculați maximul, minimul, media, mediana și abaterea standard
# corespunzătoare valorilor citite.

sprintf("Valoarea maximă este: %d", max(date))
sprintf("Valoarea minimă este: %d", min(date))
sprintf("Media este: %0.2f", mean(date))
sprintf("Mediana este: %d", median(date))
sprintf("Deviația standard este: %0.2f", sd(date))

# Sortați vectorul citit.
vector_sortat <- sort(date)
print(vector_sortat)

# Standardizați vectorul de mai sus.
standard <- c()
for (index in 1:length(date)) {
	standard[index] = date[index] - mean(date) / sd(date)
}
print(standard)
