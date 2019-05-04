from dparams import *
class PreKgcSetup:
    def __init__(self,
                 rsa_key_size = RSA_KEY_SIZE,\
                 session_key_size = SESSION_KEY_SIZE,\
                 h1_size = H1_OUTPUT_SIZE,\
                 h2_size = SESSION_KEY_SIZE,\
                 ephemeral_exponent_size = EPHEMERAL_EXPONENT_SIZE,\
                 
                 id_size = ID_SIZE,\
                 endian = ENDIAN):
        self.h1s = h1_size
        self.session_key_size = session_key_size
        self.rsa_key_size = rsa_key_size
        self.ephemeral_exponent_size = ephemeral_exponent_size
        self.id_size = id_size
        self.endian = endian

class PostKgcSetup:
    def __init__(self, preKgcSetup, N, e, g):
        self.h1s = preKgcSetup.h1s
        self.session_key_size = preKgcSetup.session_key_size 
        self.rsa_key_size = preKgcSetup.rsa_key_size
        self.ephemeral_exponent_size = preKgcSetup.ephemeral_exponent_size
        self.id_size = preKgcSetup.id_size
        self.endian = preKgcSetup.endian
        self.N = N
        self.e = e
        self.g = g

class NodeSetup:
    def __init__(self, postkgcSetup,src_port, default_dest_address, default_dest_port):
        self.src_port = src_port
        self.default_dest_address = default_dest_address
        self.default_dest_port
        self.h1s = postKgcSetup.h1s
        self.session_key_size = postKgcSetup.session_key_size 
        self.rsa_key_size = postKgcSetup.rsa_key_size
        self.ephemeral_exponent_size = postKgcSetup.ephemeral_exponent_size
        self.id_size = postKgcSetup.id_size
        self.endian = postKgcSetup.endian
        self.N = post.N
        self.e = post.e
        self.g = post.g

