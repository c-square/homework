#! /usr/bin/env python
"""Four-Russian's Method for Bit-Matrix Multiplication"""

import sys
from PyQt4 import QtGui

from fourussians.gui import MainWindow


def main():
    """Launch Four-Russians Application"""

    application = QtGui.QApplication(sys.argv)
    windows = MainWindow()
    windows.show()
    sys.exit(application.exec_())

if __name__ == "__main__":
    main()
