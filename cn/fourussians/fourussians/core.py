#! /usr/bin/env python
"""
Four-Russian's Method for Bit-Matrix Multiplication
"""

import numpy as np
    

class MatrixRow(object):

    """
    Generic representation of matrix rows
    """

    def __init__(self, items):
        """Setup new object"""
        self._items = items
        self._count = len(items)

    def __iter__(self):
        """Define Iterable Object"""
        return iter(self._items)

    def __getitem__(self, index):
        """Get specific item form row"""
        return self._items[index]

    def __setitem__(self, key, value):
        """Set specific item in specific row"""
        self._items[key] = value

    def __delitem__(self, key):
        """Delete specifc item from row"""
        del self._items[key]

    @property
    def items(self):
        """Getter for items"""
        return self._items

    @property
    def count(self):
        """Getter for count"""
        return self._count

    @items.setter
    def items(self, value):
        """Setter for items"""
        self._items = value
        self._count = len(value)

    def to_string(self, with_spaces=False):
        """Return row as string"""
        representation = []
        for element in self.items:
            representation.append(str(element))
            if with_spaces:
                representation.append(" ")

        return ''.join(representation)


class Matrix(object):

    """
    Generic matrix
    """

    def __init__(self, row=3, columns=3):
        """Setup new matrix"""
        self._rows = row
        self._columns = columns
        self._database = {}
        self._update()

    def __repr__(self):
        """Machine-readable representation"""
        return "<Matrix {}x{}>".format(self.rows, self.columns)

    def __str__(self):
        """Human-readable representation"""
        representation = []
        for index in self._database:
            row = self._database[index]
            for element in row:
                representation.append(str(element))
                representation.append(" ")
            representation.append("\n")

        return "".join(representation)

    @property
    def rows(self):
        """Getter for Bit-Matrix number of rows"""
        return self._rows

    @property
    def columns(self):
        """Getter for Bit-Matrix number of columns"""
        return self._columns

    @rows.setter
    def rows(self, value):
        """Setter for Bit-Matrix number of rows"""
        self._rows = value       # Update the value for number of rows
        self._update()           # Update matrix representation

    @columns.setter
    def columns(self, value):
        """Setter for Bit-Matrix number of columns"""
        self._columns = value    # Update the value for number of columns
        self._update()           # Update matrix representation

    def row(self, index):
        """Get specific row by index"""
        if index in self._database:
            return MatrixRow(self._database[index])

        raise ValueError("Invalid index provided !")

    def column(self, index):
        """Get specific column by index"""
        if index > self.columns:
            raise ValueError("Invalid index provided")

        result = []
        for row in self.row_iter():
            result.append(row[index])

        return result

    def row_iter(self):
        """Iterator for rows"""
        for index in range(self.rows):
            yield self.row(index)

    def set_row(self, index, row):
        """Set values for a specific row"""
        if not index in self._database:
            raise ValueError("Invalid index provided !")

        elements = []
        if type(row) in (list, tuple):
            elements.extend(row[:self.columns])

        elif type(row) == MatrixRow:
            elements.extend(row.items)

        else:
            raise ValueError("Invalid type for row !")

        for poz, val in enumerate(elements):
            self.set_item(index, poz, val)

    def set_column(self, index, values):
        """Set values for a specific column"""
        if index > self.columns:
            raise ValueError("Invalid index provided !")

        if not type(values) in (list, tuple):
            raise ValueError("Invalid type for values container !")

        for row_index, value in enumerate(values[:self.rows]):
            self.set_item(row_index, index, value)

    def set_item(self, row, column, value):
        """Set value in specific cell"""
        if not row in self._database:
            raise ValueError("Invalid row provided !")

        if column > self.columns:
            raise ValueError("Invalid column provided !")

        self._database[row][column] = value

    def _update(self):
        """
        Update matrix structure to match with the new values for rows and
        columns.
        """
        # Update the number of rows
        keys = self._database.keys()
        count = self.rows - len(keys)

        if count > 0:           # We must add `count` rows into matrix
            for index in range(count):
                self._database[len(keys) + index] = [0] * self.columns
        elif count < 0:         # We must to remove last `count` rows
            for index in keys[count:]:
                del self._database[index]

        # Update the number of columns
        for index in self._database:
            row = self._database[index]
            count = self.columns - len(row)

            if count > 0:     # We must add `count` elements
                row.extend([0] * count)
            elif count < 0:   # We must to remove last `count` elements
                del row[count:]


class BooleanMatrix(Matrix):

    """
    Boolean Matrix representation
    """

    def __init__(self, rows=3, columns=3):
        """Object setup"""
        super(BooleanMatrix, self).__init__(rows, columns)

    def __iadd__(self, other):
        """Implements addition with assignment."""
        if not type(self) == type(other):
            raise TypeError("Addition with {} is not suported !".format(
                type(other)))

        for index, row in enumerate(other.row_iter()):
            second_row = self.row(index)
            self.set_row(
                index, self._row_addition([row.items, second_row.items]))

        return self

    def __mul__(self, other):
        """Implements multiplication of Bit-Matrix."""
        if not type(self) == type(other):
            raise TypeError("Multiplication with {} is not suported !".format(
                type(other)))

        if not self.rows == other.rows:
            raise ValueError("Not suported matrix type !")

        if not self.columns == other.columns:
            raise ValueError("Not suported matrix type !")

        cache = other.cache()
        result = BooleanMatrix(self.rows, self.columns)

        for index, matrix in enumerate(self.column_split()):
            temp_matrix = BooleanMatrix(self.rows, self.columns)
            for row_index, row in enumerate(matrix.row_iter()):
                temp_matrix.set_row(row_index, cache[index][row.to_string()])

            result += temp_matrix

        return result

    def __add__(self, other):
        """Implements addition of Bit-Matrix."""
        if not type(self) == type(other):
            raise TypeError("Addition with {} is not suported !".format(
                type(other)))

        result = BooleanMatrix(self.rows, self.columns)
        for index, row in enumerate(self.row_iter()):
            second_row = other.row(index)
            result.set_row(
                index, self._row_addition([row.items, second_row.items]))

        return result

    def __repr__(self):
        """Machine-readable representation"""
        return "<Boolean Matrix {}x{}>".format(self.rows, self.columns)

    @staticmethod
    def _row_addition(rows):
        """Addition between the rows of a matrix"""
        if not type(rows) in (tuple, list):
            raise TypeError("Invalid type for rows")

        result = rows.pop()
        while len(rows):
            row = rows.pop()

            for index, value in enumerate(row):
                result[index] = result[index] or value

        return result

    def row_split(self):
        """Get all submatrix by spliting rows"""
        count = int(np.log2(self.rows))
        keys = self._database.keys()
        while len(keys):
            rows = keys[:count]
            temp_matrix = Matrix(count, self.columns)
            for index in range(count):
                if len(rows):
                    row = self.row(rows.pop())
                else:
                    row = [0] * self.columns
                temp_matrix.set_row(index, row)
            yield temp_matrix
            del keys[:count]
            del temp_matrix

    def column_split(self):
        """Get all submatrix by spliting columns"""
        count = int(np.log2(self.rows))
        keys = range(self.rows)
        while keys:
            columns = keys[:count]
            temp_matrix = Matrix(self._rows, count)
            for index in range(count):
                if columns:
                    column = self.column(columns.pop())
                else:
                    column = [0] * self.columns
                temp_matrix.set_column(index, column)

            yield temp_matrix
            del keys[:count]
            del temp_matrix

    def cache(self):
        """Generate all the posible sums with current matrix rows"""
        cache, keys, count = {}, [], int(np.log2(self.rows))
        template = "{0:0" + str(count) + "b}"

        for index in range(np.power(2, count)):
            keys.append(template.format(index))

        for group, matrix in enumerate(self.row_split()):
            cache[group] = {}
            for value in keys:
                rows = []

                for index, bit in enumerate(value):
                    if bit == "0":
                        continue
                    rows.append(matrix.row(index))

                if rows:
                    cache[group][value] = self._row_addition(rows)
                else:
                    cache[group][value] = [0] * self.columns

        return cache

    def random_values(self, set_bits=None, unset_bits=None):
        """
        Generate a matrix witch contains a `@param set_bits` bits witch are set
        or `@param unset_bits` bits witch are unset.
        """
        count = self.columns * self.rows

        if set_bits is not None and set_bits > -1:
            if set_bits > count:
                set_bits = count
            unset_bits = count - set_bits

        elif unset_bits is not None and unset_bits > -1:
            if unset_bits > count:
                unset_bits = count
            set_bits = count - unset_bits

        else:
            set_bits = np.random.random_integers(0, count)
            unset_bits = count - set_bits

        values = [0] * unset_bits
        values.extend([1] * set_bits)
        np.random.shuffle(values)

        for index in range(self.rows):
            self._database[index] = values[:self.columns]
            del values[:self.columns]
