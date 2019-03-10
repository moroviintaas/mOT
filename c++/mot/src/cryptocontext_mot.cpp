#include "include/cryptocontext_mot.h"
#include <openssl/sha.h>
/*CryptoContext_mot::CryptoContext_mot()
{

}*/


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



CryptoContext_mot::CryptoContext_mot(const SessionParameters &params)
{
    net_config = params;
}

cint CryptoContext_mot::hash1(const cint &id_to_hash) const
{
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
