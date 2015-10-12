(defrule app_meniu (not(optiune ?)) =>
	(printout t "Meniu:" crlf)
	(printout t "1. Input piesa" crlf)
	(printout t "2. Afisare piese" crlf)
	(printout t "3. Cautare titlu" crlf)
	(printout t "4. Cautare interpret" crlf)
	(printout t "5. Exit" crlf)
	(assert (optiune (read)))
)



(defrule input_piesa ?a <- (optiune 1) =>
	(retract ?a)
	(printout t "Give me the info:" crlf)
	(printout t "Format: titlu <titlu> interpret <interpret>" crlf)
	(assert (piesa (explode$(read))))
)

(defrule afisare_piese (optiune 2)(piesa titlu ?x interpret ?y) =>
	(printout t "Titlu: " ?x " Interpret: " ?y crlf)
)

(defrule delete_print ?a <- (optiune 2)=>
	(retract ?a)
)




(defrule init_cauta_titlu (optiune 3) =>
	(printout t "Titlu: ")
	(assert (cautare_t (read)))
)

(defrule cauta_titlu (optiune 3)(cautare_t $?x)(piesa titlu $?x interpret $?y) =>
	(printout t "Titlu: " ?x " Interpret: " ?y crlf)
)

(defrule delete_cautare_titlu ?b <- (optiune 3) ?a <- (cautare_t ?x) =>
	(retract ?a)
	(retract ?b)
)





(defrule init_cauta_interpret (optiune 4) =>
	(printout t "Interpret: ")
	(assert (cautare_i (read)))
)

(defrule cauta_interpret (optiune 4)(cautare_i $?x)(piesa titlu $?y interpret $?x) =>
	(printout t "Titlu: " ?y " Interpret: " ?x crlf)
)

(defrule delete_cautare_interpret ?b <- (optiune 4) ?a <- (cautare_i ?x) =>
	(retract ?a)
	(retract ?b)
)




