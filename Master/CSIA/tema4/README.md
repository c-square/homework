```
# FACULTATEA DE INFORMATICĂ, UNIVERSITATEA "AL. I. CUZA" IAȘI
# Laborator de **Capitole Speciale de Inteligență Artificială**
# Iași, 13 ianuarie 2016
```

##Tema 4∗


###Exercițiu 1

Se consideră setul de date ["House"](http://thor.info.uaic.ro/˜cgatu/csia/res/house.dat) (Sursa: Long-Kogan Realty, Chicago, USA).

Cele 14 coloane corespund, în ordine, urmatoarelor variabile:


|    |       |                                                               |
|:--:|:-----:|---------------------------------------------------------------|
|Y   | PRICE | Selling price of house in thousands of dollars                |
|X1  | BDR   | Number of bedrooms                                            |
|X2  | FLR   | Floor space in sq.ft.                                         |
|X3  | FP    | Number of fireplaces                                          |
|X4  | RMS   | Number of rooms                                               |
|X5  | ST    | Storm windows (1 if present, 0 if absent)                     |
|X6  | LOT   | Front footage of lot in feet                                  |
|X7  | TAX   | Annual taxes                                                  |
|X8  | BTH   | Number of bathrooms                                           |
|X9  | CON   | Construction (0 if frame, 1 if brick)                         |
|X10 | GAR   | Garage size (0 = no garage, 1 = one-car garage, etc.)         |
|X11 | CDN   | Condition (1 = 'need work', 0 otherwise)                      |
|X12 | L1    | Location (L1 = 1 if property is in zone A, L1 = 0 otherwise)  |
|X13 | L2    | Location (L2 = 1 if property is in zone B, L2 = 0 otherwise). |


1. Sa se determine pentru fiecare dimensiune *p* de la 1 la *n* = 13 modelul de regresie avand *p* variabile și care optimizeaza, *respectiv*, *Residual Sum of Squares* (RSS), R^2, R^2a și CP.

2. Generați graficele celor 4 criterii mai sus menționate pentru dimensiunile 1, ..., n.

3. Selectați, conform rezultatelor obținute la punctele anterioare cel mai bun (sub)model de regresie.
