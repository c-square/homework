```
# FACULTATEA DE INFORMATICĂ, UNIVERSITATEA "AL. I. CUZA" IAȘI
# Laborator de **Capitole Speciale de Inteligență Artificială**
# Iași, 14 Ianuarie 2016
```

##Tema 5∗

###Exercițiul 1

Se considera setul de date ["House"](http://thor.info.uaic.ro/˜cgatu/csia/res/house.dat) folosit în [Tema 4](../tema4) (Sursa: Long-Kogan Realty, Chicago, USA).

1. Să se determine cel mai bun (sub)model de regresie folosind metodele *forward*, *backward* și *stepwise* selection.

2. Considerați și modelul optim obținut în [Tema 4](../tema4) prin generarea exhaustiva a tuturor submodelelor posibile. Afișați într-un singur grafic și cu culori diferite, perechile de forma `(yi, yiˆ(m))`, unde i = 1, ..., 26, m = *all*, *forward*, *backward* sau *stepwise*, iar yiˆ(m) reprezintă valorile estimate (calculate) ale lui yi folosind modelul obținut prin metoda m. Ideal aceste puncte ar trebui sa se găsească pe dreapta `abline(0,1)`.

Ce puteți spune despre puterea de predicție a celor patru modele?
