(deftemplate info
	(slot nume)
	(slot val)
)

;;;;;;;; CITIRE ;;;;;;;;;;
(defrule citire
		(not (f $?))
		(not (rez $?))
		=>
	(printout t "Instruction: " )
	(assert (f (explode$ (read))))
)


;;;;;;;; CITIRE FAPT ;;;;;;;;;
(defrule citestefapt
		?a <- (f citeste fapt)
		=>
	(printout t "Reading fact: ")
	(assert (f (explode$ (read))))
	(retract ?a)
)

(defrule creare_true
		?a <- (f ?x este true)
		=>
	(assert (info (nume ?x) (val true)))
	(retract ?a)
)
(defrule creare_false
		?a <- (f ?x este false)
		=>
	(assert (info (nume ?x) (val false)))
	(retract ?a)
)


;;;;;;;; DACA ;;;;;;;;;;;
(defrule daca 
		?a <- (f daca $?x afiseaza $?y)
		=>
	(assert (rez $?x))
	(assert (mesaj $?y))
	(retract ?a)
)


;;;; SINGLE ;;;;
(defrule single_daca
		?a <- (f daca $?x)
		=>
	(assert (rez $?x))
	(assert (mesaj true))
	(retract ?a)
)


;;;;;;;; CONSTRUIRE REZULTAT ;;;;;;;;;;;
;;;; SI ;;;;
(defrule compute_si
		?a <- (rez ?x si ?y $?z)
		(info (nume ?x) (val true))
		(info (nume ?y) (val true))
		=>
	(assert (rez ?x $?z))
	(retract ?a)
)

(defrule compute_si_false
		?a <- (rez ?x si ?y $?z)
		(or 
			(info (nume ?x) (val false)) 
			(info (nume ?y) (val false))
		)
		?b <- (mesaj $?)
		=>
	(retract ?a)
	(retract ?b)
	(printout t "False" crlf)
)

;;;; OR ;;;;
(defrule compute_sau_1
		?a <- (rez ?x sau ?y $?z)
		(info (nume ?x) (val true)) 
		=>
	(assert (rez ?x $?z))
	(retract ?a)
)

(defrule compute_sau_2
		?a <- (rez ?x sau ?y $?z)
		(info (nume ?y) (val true)) 
		=>
	(assert (rez ?y $?z))
	(retract ?a)
)

(defrule compute_sau_false
	?a <- (rez ?x sau ?y $?z)
	(info (nume ?x) (val false)) 
	(info (nume ?y) (val false))
	?b <- (mesaj $?)
	=>
	(retract ?a)
	(retract ?b)
	(printout t "False" crlf)
)

;;;; SIMPLE ;;;;
(defrule simple_true
	?a <- (rez ?x)
	(info (nume ?x) (val true))
	?b <- (mesaj $?z)
	=>
	(printout t $?z crlf)
	(retract ?a)
	(retract ?b)
)

(defrule simple_false
	?a <- (rez ?x)
	(info (nume ?x) (val false))
	?b <- (mesaj $?z)
	=>
	(retract ?a)
	(retract ?b)
	(printout t "False" crlf)
)

