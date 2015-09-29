"""
    gauss.util
    ~~~~~~~~~~
    Various tools used throughout the project.
"""
import numpy as np


def precision():
    """Get machine precision"""
    power = np.float64(1)
    while True:
        test = np.power(10, -power)
        if 1 + test == 1:
            break
        power = power + 1
    value = np.power(10, -power)
    return value
