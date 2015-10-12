(deftemplate piesa
	(multislot titlu)
	(multislot interpret)
	(slot an)
)

(deffacts fapte
	(piesa (titlu Baby) (interpret Dorel) (an 1998))
	(piesa (titlu Caby) (interpret Corel) (an 2001))	
	(piesa (titlu Daby) (interpret Aurel) (an 2004))
	(lista)
)

(defrule app_meniu 
		(not(optiune ?)) 
		=>
	(printout t "Meniu:" crlf)
	(printout t "1. Input piesa" crlf)
	(printout t "2. Afisare piese" crlf)
	(printout t "3. Cautare titlu" crlf)
	(printout t "4. Cautare interpret" crlf)
	(printout t "5. Cautare an" crlf)
	(printout t "6. Sortare an" crlf)
	(printout t "7. Exit" crlf)
	(assert (optiune (read)))
)







(defrule input_piesa 
		?a <- (optiune 1) 
		=>
	(retract ?a)
	(printout t "Give me the info:" crlf)
	(printout t "Format: titlu <titlu> interpret <interpret> an <an>" crlf)
	(assert (tosave (explode$(read))))
)

(defrule save_piese 
		?a <- (tosave titlu $?x interpret $?y an ?z) 
		=>
	(assert (piesa (titlu $?x)(interpret $?y)(an ?z)))
	(retract ?a)
)







(defrule afisare_piese 
		(optiune 2)
		(piesa (titlu $?x) (interpret $?y) (an ?z)) 
		=>
	(printout t "Titlu: " $?x " Interpret: " $?y " An: " ?z crlf)
)

(defrule delete_print 
		?a <- (optiune 2)
		=>
	(retract ?a)
)







(defrule init_cauta_titlu 
		(optiune 3) 
		=>
	(printout t "Titlu: ")
	(assert (cautare_t (read)))
)

(defrule cauta_titlu 
		(optiune 3)
		(cautare_t $?x)
		(piesa (titlu $?x) (interpret $?y) (an ?z)) 
		=>
	(printout t "Titlu: " $?x " Interpret: " $?y " An: " ?z crlf)
)

(defrule delete_cautare_titlu 
		?b <- (optiune 3) 
		?a <- (cautare_t ?x) 
		=>
	(retract ?a)
	(retract ?b)
)







(defrule init_cauta_interpret 
		(optiune 4) 
		=>
	(printout t "Interpret: ")
	(assert (cautare_i (read)))
)

(defrule cauta_interpret 
		(optiune 4)
		(cautare_i $?y)
		(piesa (titlu $?x) (interpret $?y) (an ?z)) 
		=>
	(printout t "Titlu: " $?x " Interpret: " $?y " An: " ?z crlf)
)

(defrule delete_cautare_interpret 
		?b <- (optiune 4) 
		?a <- (cautare_i ?x) 
		=>
	(retract ?a)
	(retract ?b)
)






(defrule init_cauta_an 
		(optiune 5) 
		=>
	(printout t "An: ")
	(assert (cautare_a (read)))
)

(defrule cauta_an 
		(optiune 5)
		(cautare_a ?z)
		(piesa (titlu $?x) (interpret $?y) (an ?z)) 
		=>
	(printout t "Titlu: " $?x " Interpret: " $?y " An: " ?z crlf)
)

(defrule delete_cautare_an 
		?b <- (optiune 5) 
		?a <- (cautare_a ?x) 
		=>
	(retract ?a)
	(retract ?b)
)







(defrule build_list 
		(declare (salience -1))
		(optiune 6)
		?b <- (piesa (titlu $?x) (interpret $?y) (an ?z))
		?a <- (lista $?elemente) (not(lista $?start ?z $?end))
		=>
	(assert (lista $?elemente ?z))
	(retract ?a)
	(printout t "Building list with -> Titlu: " $?x " Interpret: " $?y " An: " ?z crlf)
)

(defrule bubblesort  
		(declare (salience -2))
		(optiune 6)
		?a <- (lista $?beg ?z1 ?z2 $?end) 
		(test (> ?z1 ?z2)) 
		=>
	(retract ?a)
	(assert  (lista $?beg ?z2 ?z1 $?end)  )
	(printout t "Sorting an: " ?z1 " with: " ?z2 crlf)
)

(defrule afisare  
		(declare (salience -3))
		(optiune 6)
		?b <- (lista $?start ?z $?end)
		(piesa (titlu $?x) (interpret $?y) (an ?z))
		 =>
	(printout t "Titlu: " $?x " Interpret: " $?y " An: " ?z crlf)
)

(defrule cleanup
		(declare (salience -4)) 
		?a <- (optiune 6) 
		=>
	(retract ?a)
)