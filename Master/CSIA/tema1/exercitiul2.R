#!/usr/bin/env Rscript

# Fie vectorul `x = (1, 8, 2, 6, 3, 8, 5, 5, 5, 5)`.
# Folosiți R pentru a calcula următoarele funcții.
#
x = c(1, 8, 2, 6, 3, 8, 5, 5, 5, 5)
	
punctul_1 <- function(){
	# (x1 + x2 + · · · + x10)/10.
	rezultat1 <- sum(x) / 10
	return(rezultat1)

}

punctul_2 <- function(){
	# Găsiți `log10(xi)` pentru fiecare i = 1, · · · , 10
	rezultat <- c()
	for (index in 1:length(x)) {
		rezultat[index] = log10(x[index])
	}

	return(rezultat)
}

punctul_3 <- function(){
	# Găsiți diferența între cea mai mare și cea mai mica valoare a lui x.
	diferenta <- max(x) - min(x)
	return (diferenta)
}

punctul_4 <- function(){
	# Găsiți `yi = (xi − 4.8)/2.299758` pentru fiecare i = 1, · · · , 10.
	vector <- c()
	index <- 1
	for(element in x) {
		vector[index] <- (element - 4.8) / 2.299758
		index <- index + 1
	}
	return(vector)
}

punctul_5 <- function(){
	# Calculați media și deviația standard a vectorului
	# `y = (y1, y2, · · · , y10)`.
	y <- punctul_4()
	media = mean(y)
	print(media)

	deviatia_standard = sd(y)
	print(deviatia_standard)
}

print("(x1 + x2 + · · · + x10)/10")
print(punctul_1())
print("Găsiți `log10(xi)` pentru fiecare i = 1, · · · , 10")
print(punctul_2())
print("Găsiți diferența între cea mai mare și cea mai mica valoare a lui x.")
print(punctul_3())
print("Găsiți `yi = (xi − 4.8)/2.299758` pentru fiecare i = 1, · · · , 10.")
print(punctul_4())
print("Calculați media și deviația standard a vectorului")
punctul_5()
