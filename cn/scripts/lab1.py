#! /usr/bin/env python
"""Calcul numeric - Precizie"""
import numpy as np


def evidentiaza_diferente(valoare1, valoare2):
    """Evidentiem diferentele dintre valori"""
    pozitie = 0
    for pozitie in range(len(valoare1)):
        if not valoare1[pozitie] == valoare2[pozitie]:
            break

    print "[!] Diferentele dintre rezultate"
    print "\033[92m{}\033[0m\033[91m{}\033[0m".format(valoare1[0:pozitie],
                                                      valoare1[pozitie:])
    print "\033[92m{}\033[0m\033[91m{}\033[0m".format(valoare2[0:pozitie],
                                                      valoare2[pozitie:])


def precizia_masina():
    """Obtinem precizia masina"""
    putere = np.float64(1)
    while True:
        test = np.power(10, -putere)
        if 1 + test == 1:
            break
        putere = putere + 1

    precizie = np.power(10, -putere)
    return precizie


def test_asociativitate(operatie, valoare):
    """Verificam daca operatia este asociativa"""
    precizie = precizia_masina()
    # (valoare operatie precizie) operatie precizie
    stanga = operatie(operatie(valoare, precizie), precizie)
    # valoare operatie (precizie operatie precizie)
    dreapta = operatie(valoare, operatie(precizie, precizie))

    if np.equal(stanga, dreapta):
        return True

    evidentiaza_diferente("{0:.64f}".format(stanga),
                          "{0:.64f}".format(dreapta))
    return False


def main():
    """
    Testam proprietatea de operatie asociativa pentru adunare si inmultire
    """
    operatii = {"Adunare `+`": np.add, "Inmultire `*`": np.multiply}

    for nume, operatie in operatii.iteritems():
        print "[i] Testam operatia de {} !".format(nume)

        test_values = [np.float32(1)]
        test_values.extend(list(np.random.random(100)))
        test_values.extend(list(np.random.random_integers(0, 100, 100)))

        for valoare in test_values:
            if not test_asociativitate(operatie, valoare):
                print "Precizia folosita: {}".format(precizia_masina())
                print "Valoarea folosita: {}".format(valoare)
                print "[x] {} nu este asociativa !".format(nume)
                break
        else:
            print "[i] {} este asociativa !".format(nume)
        print

if __name__ == "__main__":
    main()
