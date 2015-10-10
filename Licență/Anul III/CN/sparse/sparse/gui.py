#! /usr/bin/env python
from sparse.core import GaussSeidel
from sparse.util import parse_information
from time import time

#URL = "http://profs.info.uaic.ro/~ancai/CN/lab/4/m_rar_2014_1.txt"
URL = "http://profs.info.uaic.ro/~ancai/CN/lab/4/m_rar_2014_2.txt"


def main():
    """Test sparse module"""
    matrix, values = parse_information(URL)
    solver = GaussSeidel(matrix, values)
    matrix.sort()
    processing = time()
    print "[i] Matrix size: {}".format(matrix.size)
    print solver.solve()
    print solver.norm()
    print "[i] Done in {}".format(time() - processing)
    print "[i] Without printing"
    processing = time()
    solver.solve()
    solver.norm()
    print "[i] Done in {}".format(time() - processing)

if __name__ == "__main__":
    # print "The module has not been designed to be used in this way."
    main()
