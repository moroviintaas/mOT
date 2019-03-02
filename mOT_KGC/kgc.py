import random
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
import math
from dparams import *
from common import *
from kgc import *


class KGC_key_public:
    def __init__(self, N, e, QRN_generator, h1sf, h2f, key_size):
        self.N = N
        self.e = e
        self.QRN_generator = QRN_generator
        self.h1sf = h1sf
        self.h2f = h2f
        self.key_size = key_size


class KGC_key:
    def __init__(self, h1sf, h2f, key_size=KEY_SIZE):
        rsa_key = rsa.generate_private_key(
            public_exponent=65537,
            key_size=key_size,
            backend=default_backend()
        )
        self.key_size = key_size
        self.N = rsa_key.public_key().public_numbers().n
        self.e = rsa_key.public_key().public_numbers().e
        self.d = rsa_key.private_numbers().d
        p = rsa_key.private_numbers().p
        q = rsa_key.private_numbers().q
        self.Phi = (p - 1) * (q - 1)
        self.h1sf = h1sf  # część funkcji hashującej H (wynik całej H musi być w QRN)
        self.h2f = h2f  # druga funkcja hashująca jest w całości

    def set_QRN_generator(self, generator):
        self.QRN_generator = generator

    def public(self):
        # return (self.N,self.e,self.generator, self.h1sf, self.h2f)
        return KGC_key_public(self.N, self.e, self.QRN_generator, self.h1sf, self.h2f, self.key_size)

    # funkcja hashująca do grupy reszt kwadratowych
    def generate_user_ltk(self, user_id, endian=ENDIAN):
        # context = hashes.Hash(kgc.h1sf, backend=default_backend())
        # context.update(material)
        # exponent = int.from_bytes(context.finalize(), byteorder = endian)
        # sk = pow(kgc.QRN_generator, exponent, kgc01.N)
        hsh = hashH1(user_id, self.public(), endian)
        return pow(hsh, self.d, self.N)


