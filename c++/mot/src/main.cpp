#include <iostream>
#include <sstream>
#include <fstream>
#include "include/protocolparameters.h"
#include "include/cryptocontext_mot.h"



int main()
{
    //boost::multiprecision::mpz_int a;
    cint N, e, d,g;
    cint msgA, msgB, K_A, K_B;
    cint userA_id, userB_id, userA_pk, userB_pk;
    uint32_t rsa_key_size, session_key_size, ephemeral_exponent_size, user_id_size, h1_hash_size, h2_hash_size;
    cint test1, test2, test3;
    std::ifstream fin("test_input.txt", std::ios_base::in);
    std::ofstream bfout("test_binary_output", std::ios_base::out | std::ios_base::binary);
    std::ofstream fout("test_output.txt", std::ios_base::out);
    fin>>std::hex>>N>>e>>d>>g;

    ProtocolParameters::ReadProtocolParameters("protocol-parameters.motcfg", rsa_key_size,session_key_size,ephemeral_exponent_size,user_id_size,h1_hash_size,h2_hash_size,N,e,g);
    ProtocolParameters system_params(rsa_key_size,session_key_size,ephemeral_exponent_size,user_id_size,h1_hash_size,h2_hash_size,N,e,g);

    std::cout<<std::hex<<"N:\t"<<N<<"\n" << "e:\t"<<e<<"\n"<<"g:\t"<<g<<"\n";
    CryptoContext_mot::ReadUserDataNotEncrypted("userA-not_encrypted.motk", userA_id, userA_pk);
    CryptoContext_mot::ReadUserDataNotEncrypted("userB-not_encrypted.motk", userB_id, userB_pk);
    std::cout<<std::hex<<userA_pk<<"\n";
    CryptoContext_mot userA_context(system_params, userA_id, userA_pk);
    CryptoContext_mot userB_context(system_params, userB_id, userB_pk);
    msgA = userA_context.protocol_message_cint();
    msgB = userB_context.protocol_message_cint();

    K_A = userA_context.calculate_K(msgB,userB_id);
    K_B = userB_context.calculate_K(msgA, userA_id);


    std::cout<<"msgA:\t"<<std::hex<<msgA<<"\n";
    std::cout<<"msgB:\t"<<std::hex<<msgB<<"\n";
    std::cout<<"User A id:\t "<< std::hex<<userA_id<<"\n";
    std::cout<<"User B id:\t "<< std::hex<<userB_id<<"\n";
    std::cout<<"User A key:\t "<< std::hex<<userA_pk<<"\n";
    std::cout<<"User B key:\t "<< std::hex<<userB_pk<<"\n";
    std::cout<<"User A:K:\t "<< std::hex<<K_A<<"\n";
    std::cout<<"User B:K:\t "<< std::hex<<K_B<<"\n";


    /*
    std::cout<<std::dec<<e<<"\n";


    std::cout<<std::hex<<N.get_ui()<<"\n";

    std::cin>>std::hex>>test1;
    test2 = user_context.hash1(test1);
    std::cout<<std::hex<<test2<<"\n";

    mpz_powm_sec(test3.get_mpz_t(),test2.get_mpz_t(),d.get_mpz_t(),N.get_mpz_t());
    std::cout<<std::hex<<test3<<"\n";

    //std::cout<<"\n";
    */
    fin.close();

    return 0;
}
