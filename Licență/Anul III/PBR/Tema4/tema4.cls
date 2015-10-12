(deftemplate nod
	(slot val)
	(slot st)
	(slot dr)
)

(deffacts tosort
	(nod (val 3) (st 5) (dr 1))
	;DR
	(nod (val 5) (st 7) (dr 9))
	(nod (val 7) (st null) (dr null))
	(nod (val 9) (st null) (dr null))
	;STG
	(nod (val 1) (st 2) (dr 4))
	(nod (val 2) (st 8) (dr null))
	(nod (val 4) (st 6) (dr null))
	(nod (val 8) (st null) (dr null))
	(nod (val 6) (st null) (dr null))
)

(defrule start
	(not(faze ?))
	=>
	(assert (faze 0))
	(assert (sorted))
)

;;;;;;;;;;;;;;;  MUTARE STANGA - NO PARENT  ;;;;;;;;;;;;;;;;
(defrule verifStg (declare (salience 100))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		?b <- (nod (val ?yp) (st ?yc) (dr ?zc))
		(test (< ?xp ?yp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		=>
	(retract ?a)
	(retract ?b)
	(assert (nod (val ?yp) (st ?xp) (dr ?zp)))
	(assert (nod (val ?xp) (st ?yc) (dr ?zc)))
)

;;;;;;;;;;;;;;;  MUTARE STANGA - LA PARENT IN STANGA  ;;;;;;;;;;;;;;;;
(defrule verifStg2 (declare (salience 100))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		?b <- (nod (val ?yp) (st ?yc) (dr ?zc))
		(test (< ?xp ?yp))
		?c <- (nod (val ?ppx) (st ?xp) (dr ?ppz))
		=>
	(retract ?a)
	(retract ?b)
	(retract ?c)
	(assert (nod (val ?yp) (st ?xp) (dr ?zp)))
	(assert (nod (val ?xp) (st ?yc) (dr ?zc)))
	(assert (nod (val ?ppx) (st ?yp) (dr ?ppz)))
)

;;;;;;;;;;;;;;;  MUTARE STANGA - LA PARENT IN DREAPTA  ;;;;;;;;;;;;;;;;
(defrule verifStg3 (declare (salience 100))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		?b <- (nod (val ?yp) (st ?yc) (dr ?zc))
		(test (< ?xp ?yp))
		?c <- (nod (val ?ppx) (st ?ppy) (dr ?xp))
		=>
	(retract ?a)
	(retract ?b)
	(retract ?c)
	(assert (nod (val ?yp) (st ?xp) (dr ?zp)))
	(assert (nod (val ?xp) (st ?yc) (dr ?zc)))
	(assert (nod (val ?ppx) (st ?ppy) (dr ?yp)))
)

;;;;;;;;;;;;;;;  MUTARE DREAPTA - NO PARENT  ;;;;;;;;;;;;;;;;
(defrule verifDr (declare (salience 100))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		?b <- (nod (val ?zp) (st ?yc) (dr ?zc))
		(test (< ?xp ?zp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		=>
	(retract ?a)
	(retract ?b)
	(assert (nod (val ?zp) (st ?yp) (dr ?xp)))
	(assert (nod (val ?xp) (st ?yc) (dr ?zc)))
)

;;;;;;;;;;;;;;;  MUTARE DREAPTA - LA PARENT IN STANGA  ;;;;;;;;;;;;;;;;
(defrule verifDr2 (declare (salience 100))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		?b <- (nod (val ?zp) (st ?yc) (dr ?zc))
		(test (< ?xp ?zp))
		?c <- (nod (val ?ppx) (st ?xp) (dr ?ppz))
		=>
	(retract ?a)
	(retract ?b)
	(retract ?c)
	(assert (nod (val ?zp) (st ?yp) (dr ?xp)))
	(assert (nod (val ?xp) (st ?yc) (dr ?zc)))
	(assert (nod (val ?ppx) (st ?zp) (dr ?ppz)))
)

;;;;;;;;;;;;;;;  MUTARE DREAPTA - LA PARENT IN DREAPTA  ;;;;;;;;;;;;;;;;
(defrule verifDr3 (declare (salience 100))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		?b <- (nod (val ?zp) (st ?yc) (dr ?zc))
		(test (< ?xp ?zp))
		?c <- (nod (val ?ppx) (st ?ppy) (dr ?xp))
		=>
	(retract ?a)
	(retract ?b)
	(retract ?c)
	(assert (nod (val ?zp) (st ?yp) (dr ?xp)))
	(assert (nod (val ?xp) (st ?yc) (dr ?zc)))
	(assert (nod (val ?ppx) (st ?ppy) (dr ?zp)))
)












(defrule putInList_Single (declare (salience 70))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		(not 
			(or
				(nod (val ?yp) (st ?) (dr ?))
				(nod (val ?zp) (st ?) (dr ?))
			)
		)
		?d <- (sorted $?elements)
		=>
	(retract ?a)
	(retract ?d)
	(assert (sorted $?elements ?xp))
	(assert (mata (read)))
)

(defrule putInList_SameParent_stg (declare (salience 50))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		?b <- (nod (val ?yp) (st null) (dr null))
		?d <- (sorted $?elements)
		=>
	(retract ?a)
	(retract ?b)
	(retract ?d)

	(assert (nod (val ?yp)(st null)(dr ?zp)))
	(assert (sorted $?elements ?xp))
	(assert (mata (read)))
)

(defrule putInList_SameParent_dr (declare (salience 50))
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		?b <- (nod (val ?zp) (st null) (dr null))
		?d <- (sorted $?elements)
		=>
	(retract ?a)
	(retract ?b)
	(retract ?d)

	(assert (nod (val ?zp)(st ?yp)(dr null)))
	(assert (sorted $?elements ?xp))
	(assert (mata (read)))
)


(defrule putInListStg
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		?b <- (nod (val ?vp) (st null) (dr null))
		?c <- (nod (val ?ppx) (st ?vp) (dr ?ppz))
		?d <- (sorted $?elements)
		=>
	(modify ?a (val ?vp))
	(modify ?c (st null))
	(retract ?b)
	(retract ?d)

	(assert (sorted $?elements ?xp))
)

(defrule putInListDr
		(faze 0)
		?a <- (nod (val ?xp) (st ?yp) (dr ?zp))
		(not
			(or
				(nod (val ?) (st ?xp) (dr ?))
				(nod (val ?) (st ?) (dr ?xp))
			)
		)
		?b <- (nod (val ?vp) (st null) (dr null))
		?c <- (nod (val ?ppx) (st ?ppy) (dr ?vp))
		?d <- (sorted $?elements)
		=>
	(modify ?a (val ?vp))
	(modify ?c (dr null))
	(retract ?b)
	(retract ?d)

	(assert (sorted $?elements ?xp))
)
