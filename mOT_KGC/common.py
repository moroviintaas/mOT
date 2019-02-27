import random
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
import math


def eea(_a, _b):
    u1 = 1
    v1 = 0
    u2 = 0
    v2 = 1
    if _a < 1 or _b < 1:
        return "Error"

    if _b > _a:
        a = _b
        b = _a
    else:
        a = _a
        b = _b
    r = _b
    while r != 0:
        q = a // b

        r = a - (q * b)
        u = u1 - (q * u2)
        v = v1 - (q * v2)
        u1 = u2
        v1 = v2
        u2 = u
        v2 = v

        a = b
        b = r

        # print(q,r,u,v)
    return a, u1, v1

# dobór funkcji haschującej


def select_SHA_context(length_in_bits):
    if length_in_bits <= 224:
        return hashes.SHA224()
    elif length_in_bits <= 256:
        return hashes.SHA256()
    elif length_in_bits <= 384:
        return hashes.SHA384()
    elif length_in_bits <= 512:
        return hashes.SHA512()
    else:
        # TODO Exception
        print("Error, output to big for now")