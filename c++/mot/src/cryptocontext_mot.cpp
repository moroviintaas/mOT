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

                    }
                }
            }

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
    cint result;
    uint32_t buffer_size = net_config.get_user_id_size()/8;
    uint8_t* const message_buffer = new uint8_t [buffer_size];
    uint8_t* c_hash;
    uint32_t hash_len;
    bool success;
    const EVP_MD *evp_sha;
    if(net_config.get_user_id_size()<=256)
    {
        evp_sha = EVP_sha256();
        c_hash= new uint8_t[256/8];
    }
    else
    {
        evp_sha  =EVP_sha512();
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
        std::cout<<std::dec<<hash_len<<"\n";
        for(uint32_t i =0; i<hash_len; ++i)
        {
            std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<uint32_t(c_hash[i])<<" ";
        }
        std::cout<<"\n";

    }
    else
    {
        std::cout<<"Hash failure\n";
    }


    delete [] message_buffer;
    delete [] c_hash;

    return cint(0);
}
