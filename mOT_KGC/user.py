# from common import *
from dparams import *
from kgc import *
from motsetup import *
import common

# Użytkownik


class User:
    def __init__(self,name,user_id):
        self.name = name
        self.id = user_id 
        
    def set_private_key(self,key, postKgcSetup):
        self.private_key = key
        self.postKgcSetup = postKgcSetup 
    def generate_ephemeral_exponent(self, ephemeral_exponent_size = EPHEMERAL_EXPONENT_SIZE):
        self.ephemeral_exponent = random.randint(2**(ephemeral_exponent_size//4),2**(ephemeral_exponent_size))
    def calculate_shared_secred(self, msg, id_corr, kgc_public, endian = ENDIAN):
        hsh = hashH1(id_corr, kgc_public)
        rev_hsh = eea(kgc_public.N, hsh)[2]
        msgn_e = pow(msg, kgc_public.e, kgc_public.N)
        
        t = (msgn_e * rev_hsh)%kgc_public.N
        K = pow(t, 2*self.ephemeral_exponent, kgc_public.N)
        
        return K

    def save(self, file_name):
        with open(file_name, 'wb') as f:
            pickle.dump(self,f)

    @staticmethod
    def load(file_name):
        with open(file_name, 'rb') as f:
            u = pickle.load(f)
        return u

    def str_node_setup(self, source_port = "", dest_port = "", address = ""):
        s = """rsa_key_size={:d}
session_key_size={:d}
ephemeral_exp_size={:d}
user_id_size={:d}
h1_hash_size={:d}
h2_hash_size={:d}
modulus={}
kgc_pk={:d}
generator={}
""".format(self.postKgcSetup.rsa_key_size, self.postKgcSetup.session_key_size, self.postKgcSetup.ephemeral_exponent_size,\
           self.postKgcSetup.id_size, self.postKgcSetup.h1s,\
           self.postKgcSetup.session_key_size, hex(self.postKgcSetup.N), self.postKgcSetup.e, hex(self.postKgcSetup.g))
        if source_port != "":
            s = s + "source_port={:d}\n".format(source_port)
        if dest_port != "":
            s = s + "dest_port={:d}\n".format(dest_port)
        if address != "":
            s = s + "address={}\n".format(address)
        return s

    def str_key(self):
        s = """id:\t\t{}
sk:\t\t{}
""".format(self.id, hex(self.private_key))
        return s

    def compose_user_files(self, key_file_name = "key.motk", netcfg_file_name = "mot.cfg", src_port = "", dest_port = "", dest_address = ""):
        print ("Zapisywanie klucza do pliku:\t" + key_file_name)
        print("Zapisywanie konfiguracji do pliku:\t" + netcfg_file_name)
        with open(key_file_name, "w") as f:
            f.write("version: "+str(VERSION) + "\n" + self.str_key())
        with open(netcfg_file_name, "w") as f:
            
            f.write(self.str_node_setup(src_port, dest_port, dest_address))
            

        

    


        
