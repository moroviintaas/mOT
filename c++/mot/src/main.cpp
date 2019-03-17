#include <iostream>
#include <sstream>
#include <fstream>
#include "include/protocolparameters.h"
#include "include/cryptocontext_mot.h"
#include "include/serverinterface.h"
#include "include/clientinterface.h"
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#define SERVER "server"
#define CLIENT "client"
#define DEFAULT_KEY_FILE "key.motk"

namespace  po = boost::program_options;


void show_help(po::options_description &o)
{
    std::cout<<o<<"\n";
}
void show_version()
{
    std::cout<<"Version 0.1\n";
}

bool is_modulus_in_good_size(uint32_t modulus_size, const cint &modulus)
{
    std::string m = modulus.get_str(16);
    if (m.size()*4 > modulus_size) return false;
    else return true;
}

bool check_session_param_sizes(uint32_t rsa_key_size, uint32_t session_key_size, uint32_t ephemeral_exponent_size, uint32_t user_id_size, uint32_t h1_hash_size, uint32_t h2_hash_size)
{
    if (rsa_key_size < MIN_RSA_KEY_SIZE || session_key_size < MIN_SESSION_KEY_SIZE || ephemeral_exponent_size < MIN_EPHEMERAL_EXPONENT_SIZE || user_id_size < MIN_USER_ID_SIZE || h1_hash_size < MIN_H1_OUTPUT_SIZE || h2_hash_size < MIN_H2_OUTPUT_SIZE)
        return false;
    else return true;
}

bool check_public_crypto_params(uint32_t modulus_size, const cint &modulus, const cint &e, const cint &generator)
{
    if(is_modulus_in_good_size(modulus_size, modulus) == false) return false;
    if (modulus < 1024 || e <3 || generator < 2) return false;
    return true;
}

int main(int argc, const char* argv[])
{

    std::string cfg_file_name = "mot.cfg";
    std::string mode;
    std::string key_file_name;
    std::ifstream ifs;
    uint32_t rsa_key_size, session_key_size, ephemeral_exponent_size, user_id_size, h1_hash_size, h2_hash_size;
    cint user_id, user_sk;
    uint16_t destination_port;
    uint16_t source_port;
    std::string address;
    //std::vector<uint32_t *> v_session_param_sizes;



    std::string N_str, e_str, generator_str;
    cint N,e,generator;
    std::cin>>std::resetiosflags(std::ios_base::basefield);


    po::options_description generic("Generic");
    generic.add_options()
        ("help,h", "Display this help message")
        ("version", "Display the version number")
        ("config_file,c", po::value<std::string>(&cfg_file_name)->default_value(std::string("mot.cfg")),"Specify config file");


    po::options_description config("Configuration");
    config.add_options()
            ("modulus,N",po::value<std::string>(&N_str), "Modulus of PKI provided by KGC")
            ("kgc_pk,e", po::value<std::string>(&e_str), "Public exponent of KGC")
            ("generator,g", po::value<std::string>(&generator_str), "Generator of QRN group")
            ("keyfile,k", po::value<std::string> (&key_file_name)->default_value(std::string("key.motk")))
            ("destination_port,d", po::value<uint16_t>(&destination_port)->default_value(8080), "Destination port")
            ("source_port,s", po::value<uint16_t> (&source_port)->default_value(8081), "Source port")
            ("address,a", po::value<std::string>(&address)->default_value(std::string("127.0.0.1")), "Destination address")
            ;

    po::options_description hidden("Hidden Options");
    hidden.add_options()
            ("mode,m",po::value<std::string>(&mode), "Mode")
            ("rsa_key_size", po::value<uint32_t>(&rsa_key_size)->default_value(0), "RSA modulus size in bits")
            ("session_key_size", po::value<uint32_t>(&session_key_size)->default_value(0), "Size of session key size in bits")
            ("ephemeral_exp_size", po::value<uint32_t>(&ephemeral_exponent_size)->default_value(0), "Size of ephemeral exponent in bits")
            ("user_id_size", po::value<uint32_t>(&user_id_size)->default_value(0), "Size of user id in bits")
            ("h1_hash_size", po::value<uint32_t>(&h1_hash_size)->default_value(0), "Size of H1 hash size in bits (before squaring to QRN)")
            ("h2_hash_size", po::value<uint32_t>(&h2_hash_size)->default_value(0), "Size of H2 hash")
            ;

    po::positional_options_description pos;
    pos.add("mode", 1);

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);









    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(pos).run(), vm);
    po::notify(vm);



    ifs.open((cfg_file_name));
    po::store(po::parse_config_file(ifs, config_file_options), vm);
    po::notify(vm);
    ifs.close();


    N=N_str;
    e=e_str;
    generator = generator_str;

    if(vm.count("help"))
    {
        show_help(visible);
        return 0;
    }
    if(vm.count("version"))
    {
        show_version();
        return 0;
    }

    if(vm.count("mode"))
    {
        if (mode =="server")
        {
            CryptoContext_mot::ReadUserDataNotEncrypted(key_file_name.c_str(), user_id, user_sk);
            //std::cout<<std::hex<<user_id<<"\n";
            //std::cout<<std::hex<<user_sk<<"\n";
            if(check_session_param_sizes(rsa_key_size, session_key_size, ephemeral_exponent_size,  user_id_size, h1_hash_size,h2_hash_size) == false)
            {
                std::cout<<"Session param sizes failure!\n";
                return -1;
            }
            if(check_public_crypto_params(rsa_key_size,N,e,generator) == false)
            {
                std::cout<<"Public crypto params failure!\n";
                return -2;
            }
            ProtocolParameters system_params(rsa_key_size,session_key_size,ephemeral_exponent_size,user_id_size,h1_hash_size,h2_hash_size,N,e,generator);
            CryptoContext_mot context(system_params,user_id, user_sk);
            ServerInterface si(context,source_port);
            si.start();

            //std::cout<<std::dec<<source_port<<"\n";
        }
        if(mode == "client")
        {
            CryptoContext_mot::ReadUserDataNotEncrypted(key_file_name.c_str(), user_id, user_sk);
           // std::cout<<std::hex<<user_id<<"\n";
            //std::cout<<std::hex<<user_sk<<"\n";
            if(check_session_param_sizes(rsa_key_size, session_key_size, ephemeral_exponent_size,  user_id_size, h1_hash_size,h2_hash_size) == false)
            {
                std::cout<<"Session param sizes failure!\n";
                return -1;
            }
            if(check_public_crypto_params(rsa_key_size,N,e,generator) == false)
            {
                std::cout<<"Public crypto params failure!\n";
                return -2;
            }
            ProtocolParameters system_params(rsa_key_size,session_key_size,ephemeral_exponent_size,user_id_size,h1_hash_size,h2_hash_size,N,e,generator);
            CryptoContext_mot context(system_params,user_id, user_sk);

            //std::cout<<address<<"\n";
            ClientInterface ci(context,address,destination_port,source_port);
            ci.start();
        }
    }

    /*std::cout<<cfg_file_name<<"\n";
    std::cout<<mode<<"\n";
    std::cout<<std::hex<<N<<"\n"<<e<<"\n"<<generator<<"\n";
    std::cout<<rsa_key_size<<std::dec<<"\n"<<session_key_size<<"\n"<<ephemeral_exponent_size<<"\n"<<user_id_size<<"\n"<<h1_hash_size<<"\n"<<h2_hash_size<<"\n";

    std::cout<<key_file_name<<"\n";
    */
    /*if (argc ==1)
    {
        cint N, e,g;
        cint d;
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
        fin.close();

    }
    else if (argc >1)
    {
        cint N, e,g;
        uint32_t rsa_key_size, session_key_size, ephemeral_exponent_size, user_id_size, h1_hash_size, h2_hash_size;
        cint user_id, user_ltk;
        std::string s1 = std::string(argv[1]);
        std::vector<std::string> pargs;
        for (int i=1; i<argc; i++) pargs.push_back(argv[i]);
        if (std::string(argv[1])==SERVER)
        {
            if(argc < 3)
            {
                std::cout<<"Too few arguments for server\n";
                show_help();
            }
            else
            {
                std::cout<<argv[2]<<"\n";
                ProtocolParameters::ReadProtocolParameters("protocol-parameters.motcfg", rsa_key_size,session_key_size,ephemeral_exponent_size,user_id_size,h1_hash_size,h2_hash_size,N,e,g);
                ProtocolParameters system_params(rsa_key_size,session_key_size,ephemeral_exponent_size,user_id_size,h1_hash_size,h2_hash_size,N,e,g);

                CryptoContext_mot::ReadUserDataNotEncrypted(DEFAULT_KEY_FILE, user_id, user_ltk);
                std::cout<<std::hex<<user_id<<"\n";
                //TODO server estabilishment
            }
        }
        else if(s1==CLIENT)
        {
            if(argc < 6)
            {
                std::cout<<"Too few arguments for client\n";
                show_help();
            }
            else
            {
                std::cout<<argv[2]<<"\n";
                //TODO
            }

        }
        else
        {
            show_help();
        }
    }*/




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


    return 0;
}
