#! /usr/bin/env python
"""
    sparse.settings
    ~~~~~~~~~~~~~~~
    Various constants used throughout the project.
"""

from collections import namedtuple
import numpy


def make_constants(name, **variables):
    """Constant factory"""
    new_tuple = namedtuple(name, variables.keys())
    return new_tuple(**variables)


def precision():
    """Get current system precision"""
    index = -1
    while True:
        if not 1 + pow(10, index) > 1:
            return pow(10, index)
        index -= 1

STORE = make_constants("STORE", ROWS=0, COLUMNS=1)
SOLVE = make_constants(
    "SOLVE",
    EPSILON=precision(),
    TRIES=1000,
    DISTANCE=10 ** 16
)
