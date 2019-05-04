import random
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
import math
from dparams import *
from common import *
from user import *
from IPython import embed
from motsetup import *
import pickle

    
    


class KGC_key_public:
    def __init__(self, N, e, QRN_generator, h1sf, h2f, key_size):
        self.N = N
        self.e = e
        self.QRN_generator = QRN_generator
        self.h1sf = h1sf
        self.h2f = h2f
        self.key_size = key_size


class KGC:
    def __init__(self, h1sf, h2f,preKgcSetup = PreKgcSetup()):
        
        self.key_size = preKgcSetup.rsa_key_size

        self.p = get_secure_prime(self.key_size//16, 100)
        self.q = get_secure_prime(self.key_size//16, 100)
        self.N = self.p*self.q
        self.e = 65537
        self.h1sf = h1sf
        self.h2f = h2f
        
        self.Phi = (self.p-1)*(self.q-1)
        
        self.d = eea(self.Phi, self.e)[2]
        while self.d < 0:
            self.d += self.Phi

        #Generator:
        cont = True
        (t,r) = mr(self.Phi)
        while cont == True:
            alpha = random.randint(1,self.N)
            beta = pow(alpha,t,self.N)
            if r > 2:
                beta = pow(beta,2**(r-3),self.N)
            if  beta != 1:
                cont = False
        self.QRN_generator = pow(alpha,2,self.N)
    
            

        self.postKgcSetup = PostKgcSetup(preKgcSetup, self.N, self.e, self.QRN_generator)
    
    def set_QRN_generator(self, generator):
        #Na razie nie potrzebne - legacy
        self.QRN_generator = generator
    def public(self):
        return KGC_key_public(self.N, self.e, self.QRN_generator, self.h1sf, self.h2f, self.key_size)
    def sign_user_id(self, user_id, endian = ENDIAN):
        hsh = hashH1(user_id, self.public(), endian)
        return pow(hsh, self.d, self.N)
    #alias
    def sign (self, user_id, endian = ENDIAN):
        return self.sign_user_id(user_id, endian)

    def save(self, file_name):
        with open(file_name, 'wb') as f:
            pickle.dump(self,f)

    @staticmethod
    def load(file_name):
        with open(file_name, 'rb') as f:
            kgc = pickle.load(f)
        return kgc
    
    def addUser(self, user_name, user_id, endian = ENDIAN):
        u = User(user_name, user_id)
        u.set_private_key(self.sign(user_id,endian), self.postKgcSetup)
        return u
    

def newKGC(rsa_key_size = RSA_KEY_SIZE, h1_size=H1_OUTPUT_SIZE,h2_size = SESSION_KEY_SIZE,session_key_size=SESSION_KEY_SIZE,\
           ephemeral_exponent_size = EPHEMERAL_EXPONENT_SIZE, id_size = ID_SIZE,\
           endian = ENDIAN):
    
    preKgcSetup = PreKgcSetup(rsa_key_size,h2_size, h1_size,h2_size, ephemeral_exponent_size,\
                              id_size, endian)
    hash1_subfunction = select_SHA_context(h1_size)
    hash2_function = select_SHA_context(h2_size)
    return KGC(hash1_subfunction, hash2_function, preKgcSetup)



    


def main():
    print("Hello")
if __name__ == "__main__":
    main()
