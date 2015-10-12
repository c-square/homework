(deffacts fapte
	(PDNP Y)
	(RDIF Y)
	(PIU  Y)
)


(defrule POWER_CABLE (PDNP Y)(RLIF N)(PIU Y) =>
	(printout t "Check the power cable!" crlf)
)

(defrule PRINTER_COMPUTER (PDNP Y)(PIU Y) =>
	(printout t "Check the printer-computer cable!" crlf)
)

(defrule PRINTER_SOFTWARE (PIU Y) =>
	(printout t "Ensure printer software is installed!" crlf)
)

(defrule CHECK_INK (RLIF Y) =>
	(printout t "Check/Replace INK!" crlf)
)

(defrule CHECK_PAPER_JAMMER (PDNP Y)(PIU N) =>
	(printout t "Check paper jammer!" crlf)
)