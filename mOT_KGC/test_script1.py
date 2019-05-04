from kgc import *

k = KGC.load('k.gc')

A = k.addUser('A', 'userA@mot.pl')
A.compose_user_files(key_file_name ='userA/key.motk',\
                     netcfg_file_name='userA/mot.cfg',\
                     src_port=10011,\
                     dest_port=10010,\
                     dest_address='127.0.0.1')

B =  k.addUser('B', 'userB@mot.pl')
A.compose_user_files(key_file_name ='userB/key.motk',\
                     netcfg_file_name='userB/mot.cfg',\
                     src_port=10010,\
                     dest_port=10011,\
                     dest_address='127.0.0.1')
