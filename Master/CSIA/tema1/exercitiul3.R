#!/usr/bin/env Rscript

# Factura mobilului dumneavoastră variază lunar.
# Presupunem că anul trecut ați plătit următoarele valori lunare:
#	46 33 39 37 46 30 48 32 49 35 30 48
# Introduceți aceste valori într-o variabilă numită bill.

bill <- c(46, 33, 39, 37, 46, 30, 48, 32, 49, 35, 30, 48)
print(bill)
 
# Folosiți comada `sum` pentru a afla cuantumul anual al facturii
# telefonului mobil pe anul trecut.
sprintf("Cuantumul anual: %d", sum(bill))

# 1. Care este cea mai mică valoare plătită într-o lună?
sprintf("Valoarea minimă: %d", min(bill))

# 2. Care este cea mai mare?
sprintf("Valoarea maximă: %d", max(bill))

# 3. În cate luni factura a depășit valoarea 40.
sprintf("Numărul de valori peste 40: %d", length(bill[bill>40]))

# 4. Care a fost procentul acestora din numărul total de facturi?
sprintf("Procentajul valorilor peste 40: %0.2f%%",
		length(bill[bill>40]) * 100 / length(bill))

