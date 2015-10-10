"""
    gauss.core
    ~~~~~~~~~~
    Models for objects used throughout the project.
"""

import re
import numpy as np

from gauss.dep.matrix import Matrix
from gauss.util import precision


class Ecuation(object):

    """Generic container for a liniar ecuation"""

    PARSER = re.compile(r'([+-]*[\d\.]*)([a-z]+)')

    def __init__(self, ecuation):
        """Setup new instance"""

        self._information = {}
        self._result = 0

        if not '=' in ecuation:
            raise ValueError("Sintax Error. Missing `=` from ecuation !")

        self._ecuation = ecuation.split("=")[0]
        ecuation = ecuation.replace(' ', '')
        info = ecuation.split('=')
        ecuation = info[0]
        try:
            self._result = np.float64(info[1])
        except ValueError:
            raise ValueError("Sintax Error. Invalid value near `= {}`".format(
                info[1]))

        for value, letter in self.PARSER.findall(ecuation):
            letter = letter.strip()
            if len(value) == 0 or value in ('+', '-'):
                self._ecuation = self._ecuation.replace(letter, "1" + letter)
            try:
                if len(value) == 1 and value in ('+', '-'):
                    value += "1"
                elif len(value) == 0:
                    value = "1"

                if letter in self._information:
                    # Update value for current variable
                    self._information[letter] += np.float64(value)
                else:
                    # Set value for current variable
                    self._information[letter] = np.float64(value)
            except ValueError:
                # Invalid value provided
                raise ValueError("Sintax Error. Invalid value at `{}`".format(
                    value + letter))

        # Check if we parse all recived string
        valid = self.PARSER.sub("", ecuation)
        if valid:
            raise ValueError("Sintax Error. Invalid simbol `{}`".format(valid))

    @property
    def result(self):
        """Getter for result"""
        return self._result

    @result.setter
    def result(self, value):
        """Setter for result"""
        self._result = value

    def vars(self):
        """Returns a list of variable name"""
        return sorted(self._information.keys())

    def get(self, variable):
        """Return coefficient for recived variable name"""
        if variable in self._information:
            return self._information[variable]
        return 0

    def test(self, values):
        """Test values in ecuation"""
        ecuation = self._ecuation
        for key, value in values.iteritems():
            ecuation = ecuation.replace(key, " * {} ".format(value))
        result = eval(ecuation)
        print "[i] Ecuation after substitution:\n\t{}".format(ecuation)
        print "[i] Ecuation value: {}".format(result)
        print "[i] Expected value: {}".format(self._result)
        return result == self._result


class LiniarSystem(object):

    """Generic container for liniar ecuation"""

    def __init__(self):
        """Setup new instace"""
        self._database = Matrix(0, 0)
        self._results = {}
        self._variable = []

    def __repr__(self):
        """Machine-readable representation"""
        return "<LiniarSystem with {}>".format(self._variable)

    def __str__(self):
        """Human-readable representation"""
        representation, index = [], 0
        for var in self._variable:
            representation.append(var)
            representation.append("\t")
        representation.append("\n\n")
        for row in self._database.row_iter():
            for element in row.items:
                representation.append(str(element))
                representation.append("\t")
            representation.append("\t{}\n".format(self._results[index]))
            index += 1

        return "".join(representation)

    @property
    def results(self):
        """Getter for results"""
        return self._results

    def _update_variable(self, variable):
        """Check if system contains all required variable"""

        for var in variable:
            if not var in self._variable:
                self._database.columns += 1
                self._variable.append(var)

    def add(self, ecuation):
        """Append new ecuation into current system"""
        self._update_variable(ecuation.vars())
        row = []
        for var in self._variable:
            row.append(ecuation.get(var))

        self._database.rows += 1
        self._database.set_row(self._database.rows - 1, row)
        self._results[self._database.rows - 1] = ecuation.result

    def solve(self):
        """Solve system using Gaus elimination with partial pivoting"""
        matrix = self._database.to_list()
        resolver = Gauss(matrix, self.results)

        try:
            solution = resolver.solve()
        except ValueError:
            solution = None

        response = {}
        for index, value in enumerate(solution):
            response[self._variable[index]] = np.around(value, decimals=2)

        return response


class Gauss(object):

    """Gaussian elimination with partial pivoting
    The algorithm for solving systems of linear equations.
    """

    def __init__(self, matrix, results):
        """Setup new instance"""
        self.result = results
        self.matrix = matrix
        self._epsilon = precision()

    def swap_rows(self, pivot):
        """Determone the pivot row and swap"""
        size, length = pivot, len(self.result)

        for row in range(pivot + 1, length):
            if abs(self.matrix[row][pivot]) > abs(self.matrix[size][pivot]):
                size = row
        # swap rows from matrix
        self.matrix[pivot], self.matrix[size] = (self.matrix[size],
                                                 self.matrix[pivot])
        # swap result values
        self.result[pivot], self.result[size] = (self.result[size],
                                                 self.result[pivot])

    def assert_singular(self, pivot):
        """Check if matrix is singular or nearly singular"""
        if abs(self.matrix[pivot][pivot]) <= self._epsilon:
            raise ValueError("Matrix is singular or nearly singular")

    def triangularization(self):
        """Triangularization of matrix"""
        length = len(self.result)
        for pivot in range(length):
            self.swap_rows(pivot)
            self.assert_singular(pivot)

            for row in range(pivot + 1, length):
                alpha = self.matrix[row][pivot] / self.matrix[pivot][pivot]
                self.result[row] -= alpha * self.result[pivot]
                for column in range(pivot, length):
                    self.matrix[row][
                        column] -= alpha * self.matrix[pivot][column]

    def back_substitution(self, solution):
        """Compute solution"""
        length = len(self.result)
        solution.extend([0] * length)
        for row in range(length - 1, -1, -1):
            _sum = np.float64(0)
            for column in range(row + 1, length):
                _sum += self.matrix[row][column] * solution[column]
            solution[row] = (self.result[row] - _sum) / self.matrix[row][row]

    def solve(self):
        """Solve liniar system"""
        solution = []
        self.triangularization()
        self.back_substitution(solution)
        return solution


def main():
    """Testing module"""
    ecuation = ["b + c = 4", "2a + 4b - 2c = 2", "3b + 15c = 36"]
    container = {}
    liniar_system = LiniarSystem()
    for key in ecuation:
        container[key] = Ecuation(key)
        liniar_system.add(container[key])

    solution = liniar_system.solve()
    print "[i] Sistem solution: {}".format(sorted(solution.items()))
    for key, value in container.iteritems():
        print "[i] Ecuation: {}".format(key)
        if value.test(solution):
            print "[i] Ecuation solved successfully !"
        else:
            print "[x] Invalid solutin !"

        print

if __name__ == "__main__":
    main()
