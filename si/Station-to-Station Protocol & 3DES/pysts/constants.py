"""Constants for pySTS"""

import os
from collections import namedtuple


def make_constants(name, **variables):
    """
    Constant factory
    """
    new_tuple = namedtuple(name, variables.keys())
    return new_tuple(**variables)

GENERAL = make_constants(
    "GENERAL",
    DEBUG=True,
    GCD="lehmer",             # lehmer    | euclid
    DIVISORS="brent_rho",     # brent_rho | pollard_rho
    PRIME="deterministic",    # naive     | deterministic
    PRIME_BITES=64,           # 512 bites
    CONFIG=os.path.expanduser("~/.pysts")
)

LEHMER_GCD = make_constants(
    "LEHMER_GCD",
    DIGIT_BITS=512,
    BASE=1 << 512
)

CONFIG = make_constants(
    "CONFIG",
    CLIENT=make_constants(
        "CLIENT",
        RSA="cert-client.pem",
        STS="sts-client.json"
    ),

    SERVER=make_constants(
        "SERVER",
        RSA="cert-server.pem",
        STS="sts-server.json",
        SETUP_QUICK=False,
        WORKERS=8
    ),
)

CRYPTO = make_constants(
    "CRYPTO",
    KEY_LENGHT=make_constants(
        "KEY_LENGHT",
        RSA=4096,       # Value is in bits
        AES=24,         # Values is in bytes (192 bits)
        DES3=16         # Value is in bytes  (128 bits)
    )
)

SOCK = make_constants(
    "SOCK",
    BUFFER_SIZE=1024,
    MAX_BUFFER_SIZE=10240
)

if GENERAL.DEBUG:
    print("STS Protocol")
    print("GCD algorithm: {}".format(GENERAL.GCD))
    print("Factorization algorithm: {}".format(GENERAL.DIVISORS))
    print("Prime algorithm: {}".format(GENERAL.PRIME))
