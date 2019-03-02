#include <iostream>
#include <sstream>
#include <fstream>
#include "include/sessionparameters.h"
#include "include/cryptocontext_mot.h"



int main()
{
    cint N, e, d;
    std::ifstream fin("test_input.txt", std::ios_base::in);

    fin>>std::hex>>N>>e>>d;
    std::cout<<std::hex<<N<<"\n"<<e<<"\n"<<d<<"\n";




    return 0;
}
