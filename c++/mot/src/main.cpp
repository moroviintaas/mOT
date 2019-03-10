#include <iostream>
#include <sstream>
#include <fstream>
#include "include/sessionparameters.h"
#include "include/cryptocontext_mot.h"



int main()
{
    //boost::multiprecision::mpz_int a;
    cint N, e, d,g;
    cint test1, test2, test3;
    std::ifstream fin("test_input.txt", std::ios_base::in);
    std::ofstream bfout("test_binary_output", std::ios_base::out | std::ios_base::binary);
    std::ofstream fout("test_output.txt", std::ios_base::out);
    fin>>std::hex>>N>>e>>d>>g;
    std::cout<<std::hex<<"N:\t"<<N<<"\n" << "e:\t"<<e<<"\n"<<"d:\t"<<d<<"\n";
    SessionParameters system_params(512, 256, 256, 64, 256, 256, N,e,g);
    CryptoContext_mot user_context(system_params);



    //fin>>N>>e>>d;

    std::cout<<std::dec<<e<<"\n";
    //std::cout<<N.backend().data()->_mp_d[0]<<"\n";


    std::cout<<std::hex<<N.get_ui()<<"\n";

    std::cin>>std::hex>>test1;
    test2 = user_context.hash1(test1);
    std::cout<<std::hex<<test2<<"\n";
    //std::cout<<N.backend().data()->_mp_alloc<<"\n";

    mpz_powm_sec(test3.get_mpz_t(),test2.get_mpz_t(),d.get_mpz_t(),N.get_mpz_t());
    std::cout<<std::hex<<test3<<"\n";

    //std::cout<<"\n";
    fin.close();

    return 0;
}
