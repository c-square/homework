#! /usr/bin/env python
"""
    sparse.core
    ~~~~~~~~~~~
    Module sparse.core contains all required objects required for proper
    functioning of the sparse project.
"""

try:
    import operator
except ImportError:
    pass

import numpy
import sparse.settings as settings


class Item(object):

    """
    Implements Sparse Matrix Object
    """

    def __init__(self, index, value):
        """Setup new instance"""
        self._index = index
        self._value = numpy.float64(value)

    @property
    def index(self):
        """Getter for value"""
        return self._index

    @property
    def value(self):
        """Getter for value"""
        return self._value

    @value.setter
    def value(self, value):
        """Setter for value"""
        self._value = value


class Sparse(object):

    """
    Implements Sparse Matrix
    """

    def __init__(self, store=settings.STORE.ROWS):
        """Setup new sparse matrix"""

        if store not in settings.STORE:
            raise ValueError("invalid value provided for store.")

        self._store = store
        self._sorted = False
        self._size = tuple()
        self.__data = {}

    def __mul__(self, other):
        """Implements multimplication"""
        if not isinstance(other, Sparse):
            raise TypeError("cannot multiply 'Sparse' and '{}' objects."
                            .format(type(other)))

        first, second = self, other
        if not self.store == settings.STORE.ROWS:
            first = self.change_storage(settings.STORE.ROWS)

        if not other.store == settings.STORE.COLUMNS:
            second = other.change_storage(settings.STORE.COLUMNS)

        first.sort()
        second.sort()

        rows = first.keys()
        columns = second.keys()

        result = Sparse()
        while rows:
            row = rows.pop()
            row_values = dict(first.row(row))

            for column in columns:
                column_values = dict(second.column(column))
                value = numpy.float64(0)
                for key in set(row_values.keys()) & set(column_values.keys()):
                    value += row_values[key] * column_values[key]

                if value:
                    result.item(row, column, value)

        return result

    def _binary_search(self, row, index):
        """Binary search with deferred detection of equality"""
        items, start, end = self.__data[row], 0, len(self.__data[row])

        while start < end:
            middle = (start + end) / 2
            if not middle < end:
                break

            if items[middle].index < index:
                start = middle + 1
            else:
                end = middle

        if start == end and items[start].index == index:
            return items[start]

        return None

    def _naive_search(self, row, index):
        """Naive search for item in row"""
        if row not in self.__data:
            return None

        items, count = self.__data[row], len(self.__data[row])
        for position in range(count):
            if items[position].index == index:
                return items[position]

        return None

    def _get_size(self):
        """Get sparse matrix size"""
        if self._size:
            rows, columns = self._size
        else:
            rows, columns = 0, 0

        if self.__data:
            rows = max(rows, max(self.__data.keys()))

        if self._sorted:
            columns = max(columns, max([self.__data[key][-1].index
                                        for key in self.__data.keys()]))
        else:
            for key in self.__data:
                for item in self.__data[key]:
                    if item.index > columns:
                        columns = item.index

        if self.store == settings.STORE.COLUMNS:
            self._size = (columns, rows)
        else:
            self._size = (rows, columns)

    def _get_item(self, line, column):
        """Get specific item from matrix"""
        if self.store == settings.STORE.COLUMNS:
            line, column = column, line

        if self._sorted:
            item = self._binary_search(line, column)
        else:
            item = self._naive_search(line, column)

        return item.value if item else 0

    def _set_item(self, line, column, value):
        """Append new value"""

        if self.store == settings.STORE.COLUMNS:
            line, column = column, line

        if self._sorted:
            item = self._binary_search(line, column)
        else:
            item = self._naive_search(line, column)

        if item:
            self.handle_item_exists(item, value)
        else:
            self.__data.setdefault(line, []).append(Item(column, value))
            self._sorted = False

    @property
    def store(self):
        """Getter for store method"""
        return self._store

    @property
    def size(self):
        """Getter for size"""
        self._get_size()
        return self._size

    @size.setter
    def size(self, value):
        """Setter for size"""
        if not isinstance(value, tuple):
            raise ValueError("size should be tuple ! (rows, columns)")
        self._size = value

    def change_storage(self, store):
        """Change storage manner"""
        new_matrix = Sparse(store)
        new_matrix.size = self.size
        for line, column, value in self.items():
            new_matrix.item(line, column, value)

        return new_matrix

    def diagonal(self):
        """Returns the main diagonal of the matrix"""
        first_diagonal = []
        for index in range(min(self.size)):
            first_diagonal.append(self.item(index, index))
        return first_diagonal

    def dense(self):
        """Return the dense matrix obtain from current matrix"""

    def item(self, line, column, value=None):
        """Update element at line,column with received value"""

        if value:
            return self._set_item(line, column, value)

        return self._get_item(line, column)

    def sort(self):
        """Sort items"""

        try:
            compare = operator.attrgetter("index")
        except NameError:
            compare = lambda item: item.index

        for key in sorted(self.__data.keys()):
            sorted_row = sorted(self.__data.pop(key), key=compare)
            self.__data[key] = list(sorted_row)

        self._sorted = True

    def items(self):
        """Generator for sparse matrix items"""
        for key in self.__data.keys():
            for item in self.__data[key]:
                if self.store == settings.STORE.COLUMNS:
                    line, column = item.index, key
                else:
                    line, column = key, item.index

                yield (line, column, item.value)

    def keys(self):
        """Return available keys from internal storage"""
        return self.__data.keys()

    def row(self, index):
        """Get items from received row index"""
        _row = []
        if self.store == settings.STORE.ROWS:
            for item in self.__data.get(index, []):
                _row.append((item.index, item.value))
        else:
            for key in self.__data:
                value = self.item(index, key)
                if value:
                    _row.append((key, value))
        return _row

    def column(self, index):
        """Get items from received column index"""
        _column = []
        if self.store == settings.STORE.COLUMNS:
            for item in self.__data.get(index, []):
                _column.append((item.index, item.value))
        else:
            for key in self.__data:
                value = self.item(key, index)
                if value:
                    _column.append((key, value))

        return _column

    def random(self, density=10):
        """Generate non zero items of the received density percentage."""
        if density > 50 or density < 1:
            raise ValueError("density should be in (1-50)")

        density = density / 100.0
        limit = self.size[0] * self.size[1]
        size = min(int(limit * density), limit)

        rows = list(numpy.random.random_integers(
            low=0, high=self.size[0] - 1, size=size))
        columns = list(numpy.random.random_integers(
            low=0, high=self.size[1] - 1, size=size))

        while rows:
            value = numpy.random.random_sample() * limit
            self.item(rows.pop(), columns.pop(), value)

    @staticmethod
    def handle_item_exists(item, value):
        """Handler for set an existing item"""
        try:
            float_value = numpy.float64(value)
        except ValueError:
            raise ValueError("invalid value provided for current element.")

        item.value = item.value + float_value


class GaussSeidel(object):

    """Implements liniar system solver"""

    def __init__(self, system, values, verbose=True):
        self._system = system
        self._values = values
        self._verbose = verbose

        if not isinstance(self._values, list):
            raise TypeError("values should be a list.")

        if not isinstance(system, Sparse):
            raise TypeError("system should be a Sparse object.")

        if not self._check_diagonal():
            raise ValueError("invalid liniar system provided.")

        self.__xc = [numpy.float64(0) for _ in range(len(values))]
        self.__xp = list(self.__xc)

    def _check_diagonal(self):
        """Search for zero values in first diagonal"""
        for index, item in enumerate(self._system.diagonal()):
            if not item or item < settings.SOLVE.EPSILON:
                self.debug("Null item detected on [{},{}]."
                           .format(index, index))
                return False
        return True

    def _next_vector(self):
        """Compute array X^(k+1)"""
        for index in range(len(self._values)):
            _sum = numpy.float64(0)
            items = self._system.row(index)
            for column, value in items:
                _sum += value * self.__xc[column]

            self.__xc[index] = ((self._values[index] - _sum)
                                / self._system.item(index, index))

    def _error(self):
        """Get value for error"""
        delta = numpy.float64(0)
        for index in range(len(self.__xc)):
            delta += numpy.fabs((self.__xc[index] - self.__xp[index]) *
                                (self.__xc[index] - self.__xp[index]))
        return numpy.sqrt(delta)

    def debug(self, message, error=True):
        """Print debug messages"""
        if self._verbose:
            print "[{}] {}".format("[x]" if error else "[i]", message)

    def solve(self):
        """Implements GaussSeidel method for solving [A]{x} = {b}"""
        iteration = 0

        while self._error() < settings.SOLVE.EPSILON:
            iteration += 1
            self.__xp = list(self.__xc)  # Copy of XC object
            self._next_vector()

            # Break conditions
            if iteration > settings.SOLVE.TRIES:
                self.debug("Maximum number of tries was reached.")
                break

            """
            if self._error > settings.SOLVE.DISTANCE:
                self.debug("Maximum distance was reached !")
                break
            """
        else:
            return list(self.__xc)

        return None

    def norm(self):
        """Euclidian norm"""
        euclidian_norm = numpy.float64(0)
        _norm = [0 for _ in range(len(self.__xc))]

        for index in range(len(self.__xc)):
            items = self._system.row(index)
            for column, value in items:
                _norm[index] += value * self.__xc[column]

        for index, value in enumerate(_norm):
            euclidian_norm += numpy.fabs(
                (value - self._values[index]) * (value - self._values[index])
            )

        return numpy.sqrt(euclidian_norm)

if __name__ == "__main__":
    print "The module has not been designed to be used in this way."
