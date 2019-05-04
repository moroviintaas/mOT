import random
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
import math
from dparams import *
import os

def eea(_a,_b):
    u1 = 1
    v1 = 0
    u2 = 0
    v2 = 1
    if _a < 1 or _b < 1:
        return "Error"
    
    if _b>_a:
        a = _b
        b = _a
        reverse = False
    else:
        a = _a
        b = _b
        reverse  = True
    r = _b
    while r!=0:
        g = r
        q = a//b
        
        r = a - (q*b)   
        u = u1 - (q*u2)
        v = v1 - (q*v2)
        u1 = u2
        v1 = v2
        u2 = u
        v2 = v
        
        a = b
        b = r
        
        #print(q,r,u,v)
    if reverse:
        return (a, u1, v1)
    else:
        return (a,v1,u1)

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

def get_random(num_of_bytes):
    return int.from_bytes(os.urandom(num_of_bytes), byteorder='little')

def hashH1(material, kgc_public, endian = ENDIAN, id_size = ID_SIZE):
    material = bytes(material, encoding='utf-8')
    context = hashes.Hash(kgc_public.h1sf, backend=default_backend()) 
    context.update(material)
    hash_result = context.finalize()
    h_result = int.from_bytes(hash_result, byteorder = endian)
    hsh = pow(h_result, 2, kgc_public.N)
    return hsh

def miller_rabin_test(number, security=100):
    nm1 = number-1
    t,r = mr(nm1)
    for i in range(security):
        a = 0
        while a <2 :
            a = get_random(number.bit_length()//8+1)%number
        x = pow(a,t,number)
        if x != 1 and x != nm1:
            pt = False
            for j in range(r):
                x = pow(x,2,number)
                if x == nm1:
                    pt = True
                    break
            if pt == False:
                return False
    return True

#Rozłożenie N-1 = d* (2**r)

def get_secure_prime(num_of_bytes, security_level):
    failure_counter = 0
    next_failure_jump = 4
    success = False
    while success == False:
        p1 = get_random(num_of_bytes)
        if p1 % 2 == 0:
            p1 -= 1
        if miller_rabin_test(p1, security_level) and miller_rabin_test((2*p1)+1, security_level):
            return (2*p1)+1
        else:
            failure_counter +=1

def mr(number):
    t = number
    r = 0
    while t%2 == 0:
        t //= 2
        r += 1
    return t, r


