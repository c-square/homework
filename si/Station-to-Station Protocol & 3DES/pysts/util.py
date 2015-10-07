"""Collection of functions that will be used by the pysts module."""


from os import urandom
from time import time
from fractions import gcd as euclid_gcd

from Crypto.Util import number as nutil

from pysts.constants import GENERAL, SOCK, LEHMER_GCD


def primality_test(number):
    """Test if recived number is a prime number"""
    if GENERAL.PRIME == "deterministic":
        return is_pseudoprime(number)

    elif GENERAL.PRIME == "naive":
        return is_prime(number)

    else:
        raise ValueError("Invalid value provided for GENERAL.PRIME !")


def is_prime(number):
    """Check if :param number: is prime"""

    number = abs(number)

    if number == 2 or number == 3:
        return True

    if int(number & 1) == 0 or number % 3 == 0:
        return False

    index = 5
    increment = 2
    limit = int(number ** 0.5) + 1
    start = time()

    while index < limit:
        if number % index == 0:
            return False

        index = index + increment
        increment = 6 - increment
        if int(time() - start) > 10:
            return True

    return True


def is_pseudoprime(number):
    """
    Test whether recived number is prime using a variety of pseudoprime
    tests.
    """

    if number in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]:
        return True

    test = euler_pseudoprime(number, 2)
    if not test:
        return False

    test = euler_pseudoprime(number, 3)
    if not test:
        return False

    test = euler_pseudoprime(number, 5)
    return test


def fermat_pseudoprime(number, base):
    """
    Test whether recived number is prime or a Fermat pseudoprime to base base.
    """
    return (pow(base, number - 1, number) == 1)


def euler_pseudoprime(number, base):
    """
    Test whether number is prime or an Euler pseudoprime to base base.
    """
    if not fermat_pseudoprime(number, base):
        return False

    power = number - 1
    while (power % 2 == 0):
        power //= 2

    test = pow(base, power, number)
    if (test == 1):
        return True

    while (1):
        if (test == 1):
            return False
        if (test == number - 1):
            return True
        test = pow(test, 2, number)


def pollard_rho(number):
    """
    Pollard Rho is an integer factorization algorithm, which is quite fast
    for large numbers.
    """

    if number % 2 == 0:
        return 2

    rand_y = nutil.getRandomRange(1, number - 1, urandom)
    rand_c = nutil.getRandomRange(1, number - 1, urandom)
    copy_y = rand_y

    index_g = 1

    while index_g == 1:
        rand_y = ((rand_y * rand_y) % number + rand_c) % number
        copy_y = ((copy_y * copy_y) % number + rand_c) % number
        copy_y = ((copy_y * copy_y) % number + rand_c) % number
        index_g = gcd(abs(rand_y - copy_y), number)

    return index_g


def brent_rho(number):
    """
    Richard Brent variant of the rho algorithm

    Pollard's rho algorithm is a general-purpose integer factorization
    algorithm. It is particularly effective at splitting composite
    numbers with small factors.
    """

    if number % 2 == 0:
        return 2

    rand_y = nutil.getRandomRange(1, number - 1, urandom)
    rand_c = nutil.getRandomRange(1, number - 1, urandom)
    rand_m = nutil.getRandomRange(1, number - 1, urandom)

    index_g, index_r, index_q = 1, 1, 1

    while index_g == 1:
        copy_y = rand_y
        index_k = 0

        index = 0
        while index < index_r:
            rand_y = ((rand_y * rand_y) % number + rand_c) % number
            index += 1

        while (index_k < index_r and index_g == 1):
            y_copy2 = rand_y
            index = 0
            limit = min(rand_m, index_r - index_k)

            while index < limit:
                rand_y = ((rand_y * rand_y) % number + rand_c) % number
                index_q = index_q * (abs(copy_y - rand_y)) % number
                index += 1

            index_g = gcd(index_q, number)
            index_k = index_k + rand_m

        index_r = index_r * 2

    if index_g == number:
        while True:
            y_copy2 = ((y_copy2 * y_copy2) % number + rand_c) % number
            index_g = gcd(abs(copy_y - y_copy2), number)
            if index_g > 1:
                break

    return index_g


def lehmer_gcd(first_val, second_val):
    """Is a fast GCD algorithm, an improvement on the simpler but slower
    Euclidean algorithm. It is mainly used for big integers that have a
    representation as a string of digits relative to some chosen numeral
    system base"""

    if first_val < second_val:
        first_val, second_val = second_val, first_val

    while second_val >= LEHMER_GCD.BASE:
        size = len("{0:b}".format(first_val)) - LEHMER_GCD.DIGIT_BITS

        copy_f = int(first_val >> size)
        copy_s = int(second_val >> size)

        temp_a, temp_b, temp_c, temp_d = 1, 0, 0, 1

        while True:
            if copy_s + temp_c == 0 or copy_s + temp_d == 0:
                break

            test = (copy_f + temp_a) // (copy_s + temp_c)
            if test != (copy_f + temp_b) // (copy_s + temp_d):
                break

            temp_a = temp_c
            temp_b = temp_d
            temp_c = temp_a - test * temp_c
            temp_d = temp_b - test * temp_d

            copy_f = copy_s
            copy_s = copy_f - test * copy_s

        if temp_b:
            first_val = temp_a * first_val + temp_b * second_val
            second_val = temp_c * first_val + temp_d * second_val
        else:
            first_val, second_val = second_val, first_val % second_val

    while second_val:
        first_val, second_val = int(second_val), int(first_val % second_val)

    return first_val


def phi(number):
    """
    Euler's totient or phi function is an arithmetic function that counts
    the totatives of number
    """
    amount = 0
    index = 1
    while index < number:
        if gcd(number, index) == 1:
            amount += 1
    return amount


def gcd(first_val, second_val):
    """Returns GCD"""

    if GENERAL.GCD == "euclid":
        return euclid_gcd(first_val, second_val)
    elif GENERAL.GCD == "lehmer":
        return lehmer_gcd(first_val, second_val)
    else:
        raise ValueError("Invalid value for GENERAL.GCD")


def read_data(connection):
    """Read information from connection"""

    message, message_size = [], 0

    while message_size < SOCK.MAX_BUFFER_SIZE:
        chunk = connection.recv(SOCK.BUFFER_SIZE)
        if chunk:
            message.append(chunk)

        if len(chunk) < SOCK.BUFFER_SIZE:
            message = "".join(message)
            break

    else:
        # The buffer is too large
        raise ValueError("Request Entity Too Large")

    if not message:
        raise ValueError("No response recived !")

    return message


def debug(message):
    """If debug flag is set will print message"""
    if GENERAL.DEBUG:
        print(message)
