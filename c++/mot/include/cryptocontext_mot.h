#ifndef CRYPTOCONTEXT_MOT_H
#define CRYPTOCONTEXT_MOT_H
#include "include/sessionparameters.h"
//#include <boost/multiprecision/gmp.hpp>
#include <gmpxx.h>
#include "openssl/evp.h"
#define SIZE_UL sizeof(unsigned long)




class CryptoContext_mot
{
protected:
    SessionParameters net_config;
    cint user_id;
    cint user_ltk;

public:
    //CryptoContext_mot();
    /**
     * @brief compute_hash Computes hash function (of sha2 family), curently supports only sha256.
     * @param to_hash Buffer to be hashed.
     * @param size_of_to_hash Size of buffer (IN BYTES).
     * @param hashed Buffer to write output.
     * @param size_of_hashed Size of output buffer (IN BYTES).
     * @return Return true if hash is computed successfuly, false otherwise.
     */
    bool compute_hash( const EVP_MD* evp_sha ,uint8_t* const to_hash, uint32_t size_of_to_hash, uint8_t* hashed, uint32_t &size_of_hashed) const;
    CryptoContext_mot(const SessionParameters & params);
    /**
     * @brief hash1 Method makes hash1 function on user_id.
     * @param id_to_hash User Id to be hashed, it is of type mpz_class.
     * @return Output of hash function in type of mpz_class.
     */
    cint hash1(const cint &id_to_hash) const;

};

#endif // CRYPTOCONTEXT_MOT_H
