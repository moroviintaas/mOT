#include <iostream>
#include <sstream>
#include <fstream>
#include "include/sessionparameters.h"
#include "include/cryptocontext_mot.h"



int main()
{
    //boost::multiprecision::mpz_int a;
    cint N, e, d;
    std::ifstream fin("test_input.txt", std::ios_base::in);
    std::ofstream bfout("test_binary_output", std::ios_base::out | std::ios_base::binary);
    std::ofstream fout("test_output.txt", std::ios_base::out);

    SessionParameters system_params(512, 256, 256, 64);
    CryptoContext_mot user_context(system_params);


    fin>>std::hex>>N>>e>>d;
    //fin>>N>>e>>d;
    std::cout<<std::hex<<N<<"\n"<<e<<"\n"<<d<<"\n";
    std::cout<<std::dec<<e<<"\n";
    //std::cout<<N.backend().data()->_mp_d[0]<<"\n";


    //std::cout<<N.backend().data()->_mp_alloc<<"\n";


    //std::cout<<"\n";
    fin.close();

    return 0;
}
