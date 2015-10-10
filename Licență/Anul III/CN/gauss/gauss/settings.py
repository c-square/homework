"""
	gauss.settings
	~~~~~~~~~~~~~~
	Various constants used throughout the project.
"""

from collections import namedtuple


def make_constants(name, **variables):
    """
    Constant factory
    """
    new_tuple = namedtuple(name, variables.keys())
    return new_tuple(**variables)


