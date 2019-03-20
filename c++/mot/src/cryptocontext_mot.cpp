#include "include/cryptocontext_mot.h"
#include <openssl/sha.h>
/*CryptoContext_mot::CryptoContext_mot()
{
    valid = false;
}
*/









cint CryptoContext_mot::get_corresponder_id() const
{
    return corresponder_id;
}

void CryptoContext_mot::set_corresponder_id(const cint &value)
{
    corresponder_id = value;
}

void CryptoContext_mot::generate_session_exponent()
{
    //boost::random_device rd;
    std::random_device rd;
    std::uniform_int_distribution<uint32_t> d(0, UINT32_MAX);
    std::ifstream rin("/dev/urandom");
    ephemeral_exponent = 0;
    uint32_t full_iterations = net_config.get_ephemeral_exponent_size()/32;
    uint32_t last_iteration_size = net_config.get_ephemeral_exponent_size()%32;
    for (uint32_t i =0; i<full_iterations; i++)
    {
        ephemeral_exponent<<=32;
        ephemeral_exponent^=(d(rd)&0xffffffff);
    }
    if(last_iteration_size !=0)
    {
        ephemeral_exponent <<= last_iteration_size;
        ephemeral_exponent^=(d(rd)&0xffffffff);
    }



}

bool CryptoContext_mot::compute_hash(const EVP_MD* evp_sha, uint8_t * const to_hash, uint32_t size_of_to_hash, uint8_t *hashed, uint32_t &size_of_hashed) const
{
    bool success = false;

        EVP_MD_CTX* context = EVP_MD_CTX_new();

        if(context != nullptr)
        {
            if(EVP_DigestInit_ex(context, evp_sha, nullptr))
            {
                if(EVP_DigestUpdate(context, to_hash, size_of_to_hash))
                {
                    unsigned char hash[EVP_MAX_MD_SIZE];
                    unsigned int lengthOfHash = 0;

                    if(EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                    {
                        //std::stringstream ss;
                        if(lengthOfHash <= size_of_hashed)
                        {
                            for(unsigned int i = 0; i < lengthOfHash; ++i)
                            {
                                hashed[i] = hash[i];
                            }
                            size_of_hashed = lengthOfHash;
                            success = true;

                        }
                        else
                        {
                            std::cout<<"API length:"<<std::dec<<lengthOfHash<<"\n";
                            std::cout<<"Demanded length:"<<size_of_hashed<<"\n";
                            std::cout<<"hash fails at 1\n";
                        }

                    }
                    else std::cout<<"hash fails at 2\n";
                }
                else std::cout<<"hash fails at 3\n";
            }
            else std::cout<<"hash fails at 4\n";

            EVP_MD_CTX_free(context);
        }

        return success;
}



CryptoContext_mot::CryptoContext_mot(const ProtocolParameters &params, const cint &user_id, const cint &user_pk)
{
    net_config = params;
    this->user_id = user_id;
    this->user_sk = user_pk;

    negotiated_key = new uint8_t [params.get_session_key_size()];
    allocated_key = true;

    if(params.get_valid() && user_id >0 && user_pk > 0) valid = true;
    else valid = false;

    status = not_negotiated;

}

CryptoContext_mot::CryptoContext_mot(const CryptoContext_mot &context)
{
    /*if (allocated_key)
    {

        if(context.allocated_key)
        {
            //both allocated but with different sizes
            if (net_config.get_session_key_size() != context.net_config.get_session_key_size())
            {
                delete [] negotiated_key;
                negotiated_key = new uint8_t [context.net_config.get_session_key_size()/8];
            }
            //else is good

        }
        //else dealocate
        else
        {
            delete [] negotiated_key;
            allocated_key = false;
        }


    }
    else
    {
        if(context.allocated_key)
        {
            negotiated_key = new uint8_t [net_config.get_session_key_size()/8];
            allocated_key = true;
        }
    }
    */
    if(context.allocated_key)
    {
        negotiated_key = new uint8_t[context.net_config.get_session_key_size()/8];
        allocated_key = true;
    }


    net_config = context.net_config;
    user_id = context.user_id;
    user_sk = context.user_sk;
    ephemeral_exponent = context.ephemeral_exponent;
    corresponder_id = context.corresponder_id;
    session_id = context.session_id;



    if(allocated_key)
    {
        for (size_t i =0; i< net_config.get_session_key_size()/8; ++i)
        {
            negotiated_key[i] = context.negotiated_key[i];
        }
    }
    status = context.status;
    valid = context.valid;
}

CryptoContext_mot::~CryptoContext_mot()
{
    if(allocated_key)
    {
        delete [] negotiated_key;
    }
}

cint CryptoContext_mot::hash1(const cint &id_to_hash) const
{
    if (!valid) return -1;

    cint id = id_to_hash;
    cint result = 0;
    cint h_result=0;
    cint two = cint(2);
    std::cout<<"N:\t"<<std::hex<<net_config.get_kgc_modulus()<<"\n";
    std::cout<<id_to_hash<<"\n";
    uint32_t buffer_size = net_config.get_user_id_size()/8;
    uint8_t* const message_buffer = new uint8_t [buffer_size];
    uint8_t* c_hash;
    uint32_t hash_len = net_config.get_h1_output_size()/8;
    bool success;
    const EVP_MD *evp_sha;
    if(net_config.get_h1_output_size()<=256)
    {
        evp_sha = EVP_sha256();
        c_hash= new uint8_t[256/8];
    }
    else
    {
        evp_sha = EVP_sha512();
        c_hash = new uint8_t[512/8];
    }
    //convert input in to bytes


    /* For now I convert gmp int to bytes_array - byte by byte.
     * It may not be efficient, yet i provides correct endianness for all architectures.
     */
    for (uint32_t i =0; i < buffer_size; ++i)
    {
        message_buffer[i] = uint8_t(id.get_ui() & 0xff);
        id >>= 8;
    }

    success = compute_hash(evp_sha,message_buffer,buffer_size,c_hash, hash_len);
    if(success)
    {
        for (uint32_t i =0; i< hash_len; i++)
        {
            h_result <<= 8;
            h_result += c_hash[hash_len-1-i];
        }
        mpz_powm_sec(result.get_mpz_t(),h_result.get_mpz_t(), two.get_mpz_t(), net_config.get_kgc_modulus().get_mpz_t() );
    }
    else
    {
        std::cout<<"Hash failure\n";
    }


    delete [] message_buffer;
    delete [] c_hash;

    if(success)    return result;
    else return cint(0);
}

cint CryptoContext_mot::hash2(const cint &K_d, const cint &id_a, uint32_t bytes_of_id_a, const cint &id_b,
                              uint32_t bytes_of_id_b, const cint &msg_a, const cint &msg_b, uint32_t bytes_of_messages) const
{
    uint32_t buffer_size = (3* bytes_of_messages) + bytes_of_id_b + bytes_of_id_a;
    uint8_t *message_buffer = new uint8_t [buffer_size];
    uint8_t *c_hash;
    uint32_t hash_len = net_config.get_h2_output_size()/8;
    const EVP_MD *evp_sha;
    size_t pcount;
    bool success;
    cint result;
    uint32_t hash_size;
    if(net_config.get_h2_output_size()<=256)
    {
        evp_sha = EVP_sha256();
        c_hash= new uint8_t[256/8];
        hash_size = 256/8;
    }
    else
    {
        evp_sha = EVP_sha512();
        c_hash = new uint8_t[512/8];
        hash_size = 512/8;
    }

    mpz_export(message_buffer , &pcount, -1, sizeof (uint8_t), -1, 0, K_d.get_mpz_t());
    mpz_export(message_buffer + bytes_of_messages, &pcount, -1, sizeof (uint8_t), -1, 0, id_a.get_mpz_t());
    mpz_export(message_buffer + bytes_of_messages + bytes_of_id_a, &pcount, -1, sizeof (uint8_t), -1, 0, id_b.get_mpz_t());
    mpz_export(message_buffer + bytes_of_messages + bytes_of_id_a + bytes_of_id_b, &pcount, -1, sizeof (uint8_t), -1, 0, msg_a.get_mpz_t());
    mpz_export(message_buffer + (2*bytes_of_messages) + bytes_of_id_a + bytes_of_id_b, &pcount, -1, sizeof (uint8_t), -1, 0, msg_b.get_mpz_t());

    /*
    std::cout<<"buffer:\n";
    for(size_t i=0; i<buffer_size; ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint16_t(message_buffer[i]);
    std::cout<<"\n\n";

    std::cout<<std::dec<<bytes_of_messages<<"\n";
    for(size_t i=0; i<bytes_of_messages; ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint16_t(message_buffer[i]);
    std::cout<<"\n";
    std::cout<<std::dec<<bytes_of_id_a<<"\n";
    for(size_t i=bytes_of_messages; i<bytes_of_messages + bytes_of_id_a; ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint16_t(message_buffer[i]);
    std::cout<<"\n";
    std::cout<<std::dec<<bytes_of_id_b<<"\n";
    for(size_t i=bytes_of_messages + bytes_of_id_a; i<bytes_of_messages + bytes_of_id_a + bytes_of_id_b; ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint16_t(message_buffer[i]);
    std::cout<<"\n";
    std::cout<<std::dec<<bytes_of_messages<<"\n";
    for(size_t i=bytes_of_messages + bytes_of_id_a + bytes_of_id_b; i<(2*bytes_of_messages) + bytes_of_id_a + bytes_of_id_b; ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint16_t(message_buffer[i]);
    std::cout<<"\n";
    std::cout<<std::dec<<bytes_of_messages<<"\n";
    for(size_t i=(2*bytes_of_messages) + bytes_of_id_a + bytes_of_id_b; i<buffer_size; ++i) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint16_t(message_buffer[i]);
    std::cout<<"\n";
    */
    success = compute_hash(evp_sha,message_buffer,buffer_size,c_hash, hash_len);


    mpz_import(result.get_mpz_t(), hash_size,-1,sizeof(uint8_t), -1, 0, c_hash);
    delete [] c_hash;
    delete [] message_buffer;
    return result;
}

bool CryptoContext_mot::ReadUserDataNotEncrypted(const char *user_data_filename, cint &user_id, cint &user_sk)
{
    std::ifstream ifs(user_data_filename, std::ios_base::in);
    double version;
    std::string param_name;
    char c_buf;
    uint32_t i_buf;
    uint32_t line = 1;

    user_id = user_sk =0;

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
        if (param_name == "id:")
        {
            ifs>>std::dec>>i_buf;
            ifs>>c_buf;
            ifs>>std::hex>>user_id;
            //std::cout<<std::dec<<(int) i_buf<<"\t"<< c_buf<<"\t"<<kgc_public_exponent<<"\n";
        }
        else if (param_name == "sk:")
        {
            ifs>>std::dec>>i_buf;
            ifs>>c_buf;
            ifs>>std::hex>>user_sk;
        }
        ++line;
        ifs>>param_name;

    }
    ifs.close();
    if(user_id ==0 || user_sk == 0) return false;
    return true;

}

cint CryptoContext_mot::protocol_message_cint()
{
    if (!valid) return -1;

    cint result;
    generate_session_exponent();
    mpz_powm_sec(result.get_mpz_t(),net_config.get_generator().get_mpz_t(), ephemeral_exponent.get_mpz_t(), net_config.get_kgc_modulus().get_mpz_t());
    result = (result * user_sk) % net_config.get_kgc_modulus();
    return result;
}

cint CryptoContext_mot::calculate_K(const cint &message, const cint &corespondent_id) const
{
    if (!valid) return -1;

    cint tmp1,tmp2,tmp3, result;
    cint doubled_exponent = ephemeral_exponent * 2;
    cint user_hash = hash1(corespondent_id);

   mpz_invert(tmp1.get_mpz_t(), user_hash.get_mpz_t(), net_config.get_kgc_modulus().get_mpz_t());
   mpz_powm_sec(tmp2.get_mpz_t(), message.get_mpz_t(), net_config.get_kgc_public_exponent().get_mpz_t(), net_config.get_kgc_modulus().get_mpz_t());
   tmp3 = (tmp1 * tmp2) % net_config.get_kgc_modulus();
   mpz_powm_sec(result.get_mpz_t(), tmp3.get_mpz_t(), doubled_exponent.get_mpz_t(), net_config.get_kgc_modulus().get_mpz_t());
   return result;

}

CryptoContext_mot &CryptoContext_mot::operator=(const CryptoContext_mot &context)
{
    net_config = context.net_config;
    user_id = context.user_id;
    user_sk = context.user_sk;
    ephemeral_exponent = context.ephemeral_exponent;
    valid = context.valid;

    return *this;

}

CryptoContext_mot::session_status CryptoContext_mot::get_status() const
{
    return status;
}

time_t CryptoContext_mot::get_last_active_timestamp() const
{
    return last_active_timestamp;
}

void CryptoContext_mot::set_last_active_timestamp(const time_t &value)
{
    last_active_timestamp = value;
}

uint32_t CryptoContext_mot::get_session_id() const
{
    return session_id;
}

void CryptoContext_mot::set_session_id(const uint32_t &value)
{
    session_id = value;
}

uint16_t CryptoContext_mot::get_size_of_id_field() const
{
    if(net_config.get_user_id_size()%8==0)
        return uint16_t(net_config.get_user_id_size()/8);
    else return uint16_t(net_config.get_user_id_size()/8+1);
}

uint16_t CryptoContext_mot::get_size_of_initmsg_field() const
{
    if(net_config.get_rsa_key_size()%8==0)
        return uint16_t(net_config.get_rsa_key_size()/8);
    else return uint16_t(net_config.get_rsa_key_size()/8+1);
}
cint CryptoContext_mot::get_user_id() const
{
    return user_id;
}
