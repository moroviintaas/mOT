#include "include/protocolparameters.h"





ProtocolParameters::ProtocolParameters()
{

}

ProtocolParameters::ProtocolParameters(uint32_t rsa_key_size, uint32_t session_key_size, uint32_t ephemeral_exponent_size, uint32_t user_id_size,uint32_t h1_output_size, uint32_t h2_output_size, const cint &kgc_modulus,const cint & kgc_public_exponent, const cint & generator)
{
    set_rsa_key_size(rsa_key_size);
    set_session_key_size(session_key_size);
    set_ephemeral_exponent_size(ephemeral_exponent_size);
    set_user_id_size(user_id_size);
    set_h1_output_size(h1_output_size);
    set_h2_output_size(h2_output_size);
    set_generator(generator);
    set_kgc_public_exponent(kgc_public_exponent);
    set_kgc_modulus(kgc_modulus);
}


uint32_t ProtocolParameters::get_rsa_key_size() const
{
    return rsa_key_size;
}

void ProtocolParameters::set_rsa_key_size(const uint32_t &value)
{
    rsa_key_size = value;
}

uint32_t ProtocolParameters::get_session_key_size() const
{
    return session_key_size;
}

void ProtocolParameters::set_session_key_size(const uint32_t &value)
{
    session_key_size = value;
}

uint32_t ProtocolParameters::get_ephemeral_exponent_size() const
{
    return ephemeral_exponent_size;
}

void ProtocolParameters::set_ephemeral_exponent_size(const uint32_t &value)
{
    ephemeral_exponent_size = value;
}

uint32_t ProtocolParameters::get_user_id_size() const
{
    return user_id_size;
}

void ProtocolParameters::set_user_id_size(const uint32_t &value)
{
    user_id_size = value;
}
cint ProtocolParameters::get_generator() const
{
    return generator;
}

void ProtocolParameters::set_generator(const cint &value)
{
    generator = value;
}

uint32_t ProtocolParameters::get_h1_output_size() const
{
    return h1_output_size;
}

void ProtocolParameters::set_h1_output_size(const uint32_t &value)
{
    h1_output_size = value;
}

uint32_t ProtocolParameters::get_h2_output_size() const
{
    return h2_output_size;
}

void ProtocolParameters::set_h2_output_size(const uint32_t &value)
{
    h2_output_size = value;
}

cint ProtocolParameters::get_kgc_public_exponent() const
{
    return kgc_public_exponent;
}

void ProtocolParameters::set_kgc_public_exponent(const cint &value)
{
    kgc_public_exponent = value;
}

cint ProtocolParameters::get_kgc_modulus() const
{
    return kgc_modulus;
}

void ProtocolParameters::set_kgc_modulus(const cint &value)
{
    kgc_modulus = value;
}

ProtocolParameters &ProtocolParameters::operator=(const ProtocolParameters &params)
{
    set_rsa_key_size(params.get_rsa_key_size());
    set_session_key_size(params.get_session_key_size());
    set_ephemeral_exponent_size(params.get_ephemeral_exponent_size());
    set_user_id_size(params.get_user_id_size());
    set_h1_output_size(params.get_h1_output_size());
    set_h2_output_size(params.get_h2_output_size());
    set_kgc_modulus(params.get_kgc_modulus());
    set_kgc_public_exponent(params.get_kgc_public_exponent());
    set_generator(params.get_generator());
    return *this;
}

bool ProtocolParameters::ReadProtocolParameters(const char *config_file_name, uint32_t &rsa_key_size, uint32_t &session_key_size, uint32_t &ephemeral_exponent_size, uint32_t &user_id_size, uint32_t &h1_hash_size, uint32_t &h2_hash_size, cint &kgc_modulus, cint &kgc_public_exponent, cint &generator)
{
        std::ifstream ifs(config_file_name, std::ios_base::in);
        double version;
        std::string param_name;
        char c_buf;
        uint32_t i_buf;
        uint32_t line = 1;

        rsa_key_size = session_key_size = ephemeral_exponent_size = user_id_size = h1_hash_size = h2_hash_size = 0;
        kgc_modulus = kgc_public_exponent = generator =0;

        ifs>>param_name;
        if (param_name != "version:")
        {
            ifs.close();
            return false;
        }
        ifs>>version;
        line = 2;

        ifs>>param_name;
        while (ifs.good())
        {
            if (param_name == "rsa_key_size:") ifs>>std::dec>>rsa_key_size;
            else if (param_name == "session_key_size:") ifs>>std::dec>>session_key_size;
            else if (param_name == "ephemeral_exp_size:") ifs>>std::dec>>ephemeral_exponent_size;
            else if (param_name == "user_id_size:") ifs>>std::dec>>user_id_size;
            else if (param_name == "h1_hash_size:") ifs>>std::dec>>h1_hash_size;
            else if (param_name == "h2_hash_size:") ifs>>std::dec>>h2_hash_size;
            else if (param_name == "modulus:")
            {
                ifs>>std::dec>>i_buf;
                ifs>>c_buf;
                ifs>>std::hex>>kgc_modulus;
            }
            else if (param_name == "public_exponent:")
            {
                ifs>>std::dec>>i_buf;
                ifs>>c_buf;
                ifs>>std::hex>>kgc_public_exponent;
                //std::cout<<std::dec<<(int) i_buf<<"\t"<< c_buf<<"\t"<<kgc_public_exponent<<"\n";
            }
            else if (param_name == "generator:")
            {
                ifs>>std::dec>>i_buf;
                ifs>>c_buf;
                ifs>>std::hex>>generator;
            }
            else std::cout<<"error in line:\t" << line<<"\n";
            ++line;
            ifs>>param_name;
        }
        ifs.close();

        if(rsa_key_size == 0 || session_key_size == 0 || ephemeral_exponent_size == 0 || user_id_size ==0 || h1_hash_size ==0 || h2_hash_size == 0 || kgc_modulus == 0 || kgc_public_exponent == 0 || generator == 0)
            return false;
        return true;

}

