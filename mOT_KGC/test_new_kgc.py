from kgc import *

k = newKGC(rsa_key_size=1024,\
           h1_size=256,\
           h2_size=256,\
           session_key_size=256,\
           ephemeral_exponent_size = 160,\
           id_size = 512)

k.save("k.gc")
