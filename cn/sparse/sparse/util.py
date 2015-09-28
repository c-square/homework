#! /usr/bin/env python

import numpy
import urllib2
from decimal import getcontext
from sparse.core import Sparse


def download_matrix(url):
    """Download information from url"""
    request = urllib2.Request(url)
    try:
        request_handler = urllib2.urlopen(request)
    except urllib2.HTTPError as exc:
        print "[x] Fail to download information. {}".format(exc)
        return

    file_content = request_handler.readlines()
    yield file_content[0]
    for line in file_content[2:]:
        if not line:
            continue
        yield line.strip()


def parse_information(url):
    """Generate new sparse matrix"""
    values = []
    matrix = Sparse()
    information = download_matrix(url)
    size = None
    for info in information:
        if ',' not in info:
            if not size:
                size = int(info.strip())
            else:
                try:
                    values.append(numpy.float64(info))
                except ValueError:
                    pass
        else:
            unpack = info.split(',')
            if not len(unpack) == 3:
                continue

            matrix.item(int(unpack[1].strip()), int(unpack[2].strip()),
                        unpack[0].strip())

    matrix.size = (size, size)
    matrix.sort()
    return matrix, values

if __name__ == "__main__":
    print "The module has not been designed to be used in this way."
