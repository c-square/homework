"""
fourussians.gui
    User Interface for `Four-Russian's Method for Bit-Matrix Multiplication`
"""
import sys
from PyQt4 import QtGui, QtCore
from fourussians.core import BooleanMatrix

SIZE = 16


class MainWindow(QtGui.QWidget):

    """
    Four-Russian's Method - Main Window
    """

    def __init__(self, parent=None):
        """Setup widget"""

        QtGui.QWidget.__init__(self, parent)

        self.setWindowTitle("Four-Russian's Method")
        # self.setWindowIcon(QtGui.QIcon("icon.png"))

        # Set layout
        self._tabs = QtGui.QTabWidget()
        self.matrix_a = BooleanMatrix(SIZE, SIZE)
        self.matrix_b = BooleanMatrix(SIZE, SIZE)
        self.matrix_c = BooleanMatrix(SIZE, SIZE)

        # Objects
        self._matrix_a = QtGui.QTableWidget()
        self._matrix_b = QtGui.QTableWidget()
        self._matrix_c = QtGui.QTableWidget()

        self.setGeometry(0, 0, 500, 500)
        self.resize(500, 500)
        self.setMinimumSize(500, 500)
        self.center()

        # Add tabs to tabs layout
        for element in dir(self):
            if not element.startswith("_tab_"):
                continue
            getattr(self, element)()

        window_layout = QtGui.QVBoxLayout()
        window_layout.addWidget(self._tabs)

        # Init matrix
        self.init_matrix(self.matrix_a, self._matrix_a)
        self.init_matrix(self.matrix_b, self._matrix_b)
        self.init_matrix(self.matrix_c, self._matrix_c)

        self.setLayout(window_layout)

    def compute_multiplication(self):
        """Compute mutliplication of matrix"""
        self.update_matrix(self.matrix_a, self._matrix_a)
        self.update_matrix(self.matrix_b, self._matrix_b)

        self.matrix_c = self.matrix_a * self.matrix_b
        # print self.matrix_c
        self.update_table(self.matrix_c, self._matrix_c)

    @staticmethod
    def init_matrix(matrix, table):
        """Init matrix"""
        for row in range(matrix.rows):
            for column in range(matrix.columns):
                value = matrix.row(row)[column]
                # print "[i] Set {} to ({},{})".format(value, row, column)
                table.setItem(row, column, QtGui.QTableWidgetItem(str(value)))

    def randomize(self, matrix, table):
        """Randomize table"""
        matrix.random_values()
        self.update_table(matrix, table)

    @staticmethod
    def update_table(matrix, table):
        """Syncronize matrix with table"""
        for row in range(matrix.rows):
            for column in range(matrix.columns):
                value = matrix.row(row)[column]
                # print "[i] Set {} to ({},{})".format(value, row, column)
                table.setItem(row, column, QtGui.QTableWidgetItem(str(value)))

    @staticmethod
    def update_matrix(matrix, table):
        """Syncronize matrix with table"""

        for row in range(matrix.rows):
            for column in range(matrix.columns):
                element = table.item(row, column).text()
                matrix.set_item(row, column, int(element))

    def _tab_1_main(self):
        """Main tab"""
        tab = QtGui.QWidget()
        layout = QtGui.QVBoxLayout(tab)
        readme = QtGui.QTextBrowser()
        try:
            content = ["Four-Russian's Method for Bit-Matrix Multiplication\n"]
            file_handler = open("ext/README", "rb")
            while True:
                temp = file_handler.read(1024)
                if not temp:
                    break
                content.append(temp)
            file_handler.close()
        except IOError as exc:
            print>>sys.stderr, "[x] Fail to read README file. {}".format(exc)

        readme.setText(''.join(content))
        layout.addWidget(readme)
        self._tabs.addTab(tab, "Welcome")

    def _tab_1st_matrix(self):
        """First matrix tab"""
        tab = QtGui.QWidget()
        layout = QtGui.QVBoxLayout(tab)

        self._matrix_a.setRowCount(SIZE)
        self._matrix_a.setColumnCount(SIZE)

        self._matrix_a.resizeColumnsToContents()
        self._matrix_a.resizeRowsToContents()

        btn_randomize = QtGui.QPushButton("Valori aleatorii")
        self.connect(btn_randomize, QtCore.SIGNAL(
            "released()"),
            lambda: self.randomize(self.matrix_a, self._matrix_a))

        layout.addWidget(self._matrix_a)
        layout.addWidget(btn_randomize)
        self._tabs.addTab(tab, "First Matrix")

    def _tab_2nd_matrix(self):
        """Second matrix tab"""
        tab = QtGui.QWidget()
        layout = QtGui.QVBoxLayout(tab)

        self._matrix_b.setRowCount(SIZE)
        self._matrix_b.setColumnCount(SIZE)
        btn_randomize = QtGui.QPushButton("Valori aleatorii")

        self._matrix_b.resizeColumnsToContents()
        self._matrix_b.resizeRowsToContents()

        self.connect(btn_randomize, QtCore.SIGNAL("released()"),
                     lambda: self.randomize(self.matrix_b, self._matrix_b))

        layout.addWidget(self._matrix_b)
        layout.addWidget(btn_randomize)
        self._tabs.addTab(tab, "Second Matrix")

    def _tab_result(self):
        """Second matrix tab"""
        tab = QtGui.QWidget()
        layout = QtGui.QVBoxLayout(tab)
        btn_compute = QtGui.QPushButton("Actualizeaza valori")

        self._matrix_c.setRowCount(SIZE)
        self._matrix_c.setColumnCount(SIZE)

        self._matrix_c.resizeColumnsToContents()
        self._matrix_c.resizeRowsToContents()

        self.connect(btn_compute, QtCore.SIGNAL("released()"),
                     self.compute_multiplication)

        layout.addWidget(self._matrix_c)
        layout.addWidget(btn_compute)
        self._tabs.addTab(tab, "Second Matrix")

    def center(self):
        """Move window to center of screen"""
        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.frameSize()
        self.move((screen.width() - size.width()) / 2,
                  (screen.height() - size.height()) / 2)

if __name__ == "__main__":
    print "[x] Nothing to see here !"
