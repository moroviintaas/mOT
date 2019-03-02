# from common import *
from dparams import *
import common

# Użytkownik


class User:
    def __init__(self, name, user_id):
        self.name = name
        self.id = user_id  # user_id.to_bytes(ID_SIZE//8, byteorder=ENDIAN)
        self.ephemeral_exponent = b''
        self.private_key = b''
        # self.idn = int.from_bytes(self.id, byteorder=ENDIAN)

    def set_private_key(self, key, endian=ENDIAN):
        self.private_key = key
        # self.private_key_n = int.from_bytes(key, endian)

    def generate_ephemeral_exponent(self, ephemeral_exponent_size=EPHEMERAL_EXPONENT_SIZE):
        self.ephemeral_exponent = random.randint(2 ** (ephemeral_exponent_size // 4), 2 ** (ephemeral_exponent_size))

    def calculate_shared_secred(self, msg, id_corr, kgc_public, endian=ENDIAN, id_size=ID_SIZE):
        # print("id_corr", id_corr)
        # hsh = hashH1(corr_idn.to_bytes(kgc_public.key_size//8, endian), kgc_public)
        hsh = common.hashH1(id_corr, kgc_public, endian, id_size)
        rev_hsh = common.eea(kgc_public.N, hsh)[2]

        print("with reversing hsh:", hsh * rev_hsh % kgc_public.N)

        # msgn = int.from_bytes(msg,endian)
        # msgn_e = pow(msgn, kgc_public.e, kgc_public.N)
        msgn_e = pow(msg, kgc_public.e, kgc_public.N)

        t = (msgn_e * rev_hsh) % kgc_public.N
        K = pow(t, 2 * self.ephemeral_exponent, kgc_public.N)

        return K
