"""
Inteligenta Artificiala - Problema damelor
"""

from os import makedirs
from os.path import isfile, isdir, dirname, abspath
from json import dumps, loads
from hashlib import md5
from argparse import ArgumentParser, FileType
from random import randrange

# Variabile globale
FLAG_VERBOSE = False


def verbose(message):
    """
    In cazul in care este setat --verbose va printa pe ecran
    mesajul primit ca argument
    """

    if FLAG_VERBOSE:
        print message


class Regine(object):

    """
    Obiect ce va incerca sa rezolve problema reginelor pe o tabla
    de NxN.
    """

    CACHE_PATH = abspath(r'cache.json')

    def __init__(self, instanta, use_cache=True, output=None, learn=False):
        """
        Initializarea obiectului
        """

        self.__cache = use_cache
        self._output = output
        self._learn = learn
        self._cache = {}

        self._instanta = instanta
        self._stare_curenta = []

        if self.__cache:
            if not isdir(dirname(Regine.CACHE_PATH)):
                try:
                    makedirs(dirname(Regine.CACHE_PATH))
                except IOError as _:
                    raise IOError("[x] Nu pot crea folderul pentru cache !")

            self._load_cache()

    def __del__(self):
        """
        Distrugerea obiectului
        """

        if self.__cache:
            self._dump_cache()

    def _write(self, message):
        """
        Scriem in fisierul de output.

        Formatul va fi [tranzitie] : [mesaj]
        """

        if self._output:
            try:
                self._output.write(
                    "[{tranzitie}] : [{mesaj}]\n".format(
                        mesaj=message, tranzitie=self._to_string()))
            except IOError as exc:
                print "[x] Eroare la scriere in fisier: {}".format(exc)

    def stare_finala(self):
        """
        Verificam daca am ajuns intr-o stare finala.

        Daca starea curenta contine toate coordonatele pentru
        regine atunci spunem ca ne aflam intr-o stare finala.
        """

        if len(self._stare_curenta) == self._instanta:
            return True

        return False

    def validare_tranzitie(self):
        """
        Verificam daca starea posibila obtinuta in urma tranzitie
        este si una valida.
        """

        in_cache = self.in_cache()
        raspuns = True

        # Verificam daca cunoastem deja aceasta informatie
        if not in_cache is None:
            return in_cache

        stare_posibila = self._stare_curenta[-1]

        for stare_valida in self._stare_curenta[:-1]:
            if stare_posibila[0] == stare_valida[0]:
                # Nu poate fi pozitionata o regina ce are aceeasi coordonata
                # X cu una deja existenta pe tabla.
                raspuns = False
                break

            elif stare_posibila[1] == stare_valida[1]:
                # Nu poate fi pozitionata o regina ce are aceeasi coordonata
                # X cu una deja existenta pe tabla.
                raspuns = False
                break

            elif abs(stare_posibila[0] - stare_valida[0]) == abs(
                    stare_posibila[1] - stare_valida[1]):
                # Nu poate fi pozitionata o regina la o locatie care are pe
                # una dintre diagonale o alta regina
                raspuns = False
                break

        # Adaugam raspunsul in baza de cunostiinte
        self.invata(raspuns)

        return raspuns

    def strategie(self, metoda):
        """
        Alegem strategia folosita de acest obiect
        """

        if metoda:
            self._strategie_random()
        else:
            self._strategie_interativa()

    def _strategie_interativa(self):
        """
        Strategie iterativa.
        """

        stare = None

        while not self.stare_finala():

            generator_tranzactii = self._tranzitie_iterativa(stare)
            anti_bucla = True
            stare = None

            for _ in generator_tranzactii:
                if not self.validare_tranzitie():
                    self._write("Invalida")
                    try:
                        self._stare_curenta.pop()
                    except IndexError:
                        pass
                else:
                    self._write("Valida")
                    anti_bucla = False
                    break

            if anti_bucla:
                # Alegerea anterioara a fost eronata
                verbose("Mecanismul antibucla a fost activa."
                        "Invalidam starea precedenta")

                while anti_bucla:
                    # Facem roolback la ultima configuratie stabila
                    try:
                        stare = self._stare_curenta.pop()
                    except IndexError:
                        verbose("Problema nu are solutie !")
                        self._write("Fara solutie !")
                        return False

                    if not stare[1] == self._instanta:
                        stare = stare[1] + 1
                        anti_bucla = False

                    # Actualizam cache
                    self.invata(False)

        self._write("Solutie")

    def _tranzitie_iterativa(self, offset=None):
        """
        Pornind de la o stare curenta o sa deducem care ar fi
        urmatoarea stare posibila.

        Nota:
            Generator de stari posibile.
        """

        regina = [len(self._stare_curenta) + 1, 1]
        offset = offset or 1

        for coord_y in xrange(offset, self._instanta + 1):
            regina[1] = coord_y
            self._stare_curenta.append(regina)
            yield

    def _strategie_random(self):
        """
        Strategia abordata incearca in egala masura sa fie una
        optima si sa evite buclele infinite.
        """
        istoric = {0: set()}

        while not self.stare_finala():

            anti_bucla = True

            while True:
                if len(istoric[len(self._stare_curenta)]) == self._instanta:
                    break

                self._tranzitie_random()

                if not self.validare_tranzitie():
                    self._write("Invalida")
                    try:
                        value = self._stare_curenta.pop()[1]
                        istoric[len(self._stare_curenta)].add(value)
                    except IndexError:
                        pass
                else:
                    self._write("Valida")
                    istoric[len(self._stare_curenta)] = set()
                    anti_bucla = False
                    break

            if anti_bucla:
                # Alegerea anterioara a fost eronata
                verbose('-' * 65)
                verbose("[i]Mecanismul antibucla a fost activa."
                        "Invalidam starea precedenta")
                verbose(
                    '\n'.join(["{} -> {}".format(
                        key, value) for key, value in istoric.iteritems()]))

                verbose('-' * 65)

                while anti_bucla:
                    # Facem roolback la ultima configuratie stabila
                    try:
                        if len(self._stare_curenta) == 1:
                            if not len(istoric[len(self._stare_curenta)]) \
                                    == self._instanta:
                                break
                        istoric[len(self._stare_curenta)] = set()
                        self._stare_curenta.pop()
                    except IndexError:
                        verbose("Problema nu are solutie !")
                        self._write("Fara solutie !")
                        return False

                    if len(istoric[len(
                            self._stare_curenta)]) == self._instanta:
                        anti_bucla = False

                    # Actualizam cache
                    self.invata(False)

        self._write("Solutie")
        verbose("Solutie este: {}".format(self._to_string()))

    def _tranzitie_random(self):
        """
        Pornind de la starea curenta o sa incercam sa alegem
        un numar aleatoriu in intervalul posibil
        """
        regina = [len(self._stare_curenta) + 1, 1]

        regina[1] = randrange(1, self._instanta + 1)
        self._stare_curenta.append(regina)

    def _load_cache(self):
        """
        Incarcam valorile stocate pe disk
        """

        if not isfile(Regine.CACHE_PATH):
            return

        try:
            file_handler = open(Regine.CACHE_PATH, 'rb')
            file_content = file_handler.read()
            file_handler.close()
        except IOError as _:
            print "[x] Nu am putut incarca informatiile din cache !"
            return

        try:
            self._cache = loads(file_content)
        except ValueError:
            print "[x] Informatiile din cache sunt corupte !"
            return

    def _dump_cache(self):
        """
        Exportam valorile stocate in cache
        """

        try:

            file_handler = open(Regine.CACHE_PATH, 'wb')
            file_handler.write(dumps(self._cache))
            file_handler.close()

        except ValueError as exc:
            print "[x] Nu am putut exporta cache-ul pentru ca {}".format(exc)

        except IOError as exc:
            print "[x] Nu am putut exporta cache-ul pentru ca {}".format(exc)

    def invata(self, value):
        """
        Adaugam o noua informatie in baza de cunostiinte
        """

        if not self.__cache:
            return

        stare = md5(self._to_string()).hexdigest()
        self._cache[stare] = value

        verbose("[i] Adaug in cache starea:\n\t{} => {}:{}".format(
            self._stare_curenta, stare, value))

    def in_cache(self):
        """
        Verificam daca avem in baza de cunostiinte aceasta tranzitie
        """

        if not self.__cache:
            return None

        stare = md5(self._to_string()).hexdigest()

        if stare in self._cache:
            verbose("[i] Starea este in cache:\n\t{} => {}:{}".format(
                self._stare_curenta, stare, self._cache[stare]))

            return self._cache[stare]

        verbose("[w] Starea nu a fost gasita in cache:\n\t{} => {}:{}".format(
            self._stare_curenta, stare, 'n/a'))

        return None

    def start(self, mod):
        """
        Alegem modul in care se doreste rezolvarea problemei.
        """

        if not type(mod) == bool and mod not in (0, 1):
            raise RuntimeError("Valoare invalida pentru mod!")

        if mod:
            # Utilizatorul doreste rezolvarea problemei folosind
            # strategia random
            verbose("[i] Utilizatorul a ales strategia random.")

        else:
            # Utilizatorul doreste rezolvarea problemei folosind
            # strategia iterativa
            verbose("[i] Utilizatorul a ales strategia iterativa.")

        self.strategie(mod)

    def _to_string(self):
        """
        Reprezinta starea curenta ca string
        """
        return dumps(
            (self._instanta, self._stare_curenta)
        ).replace("[", "").replace("]", "").replace(" ", "")


def main():
    """
    Procesam informatiile primite de la utilizator
    """

    global FLAG_VERBOSE

    parser = ArgumentParser(
        description="Programul se ocupa cu rezolvarea problemei damelor."
    )
    group = parser.add_mutually_exclusive_group()

    group.add_argument(
        "-v", "--verbose", action="store_true",
        help="In cazul in care este setat se vor afisa mesaje privitoare la"
              "deciziile pe care le ia aplicatia.")

    parser.add_argument(
        "instanta",
        type=int,
        help="Instanta pentru problema propusa. In cazul problemei de fata "
             "numarul de regine ce vor fi pe tabla."
    )

    parser.add_argument(
        "-r", "--random",
        action="store_true",
        help="Abordarea unei strategii random.",
    )

    parser.add_argument(
        "-c", "--cache",
        action="store_true",
        help="Folosirea sau nu a unui sistem de retinere a informatiilor.",
    )

    parser.add_argument(
        "-l", "--learn",
        action="store_true",
        help="Aplicatia va incerca sa gaseasca o solutie noua."
    )

    parser.add_argument(
        "-o", "--output",
        type=FileType('wb'),
        help="Fisierul in care se vor scrie rezultatele obtinute."
    )

    args = parser.parse_args()

    FLAG_VERBOSE = args.verbose

    if args.learn:
        verbose("[w] Nu este functional in aceasta versiune")

    problema_regine = Regine(args.instanta, args.cache, args.output)
    problema_regine.start(args.random)

if __name__ == "__main__":
    # Incepem executia
    main()
