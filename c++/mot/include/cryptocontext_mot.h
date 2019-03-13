#ifndef CRYPTOCONTEXT_MOT_H
#define CRYPTOCONTEXT_MOT_H
#include "include/protocolparameters.h"
//#include <boost/multiprecision/gmp.hpp>
#include <gmpxx.h>
#include "openssl/evp.h"
#include <random>
#define SIZE_UL sizeof(unsigned long)




class CryptoContext_mot
{
protected:
    ProtocolParameters net_config;
    cint user_id;
    cint user_ltk;
    cint ephemeral_exponent;
    /**
     * @brief generate_session_exponent Generates ephemeral exponent using boost:random_device.
     */
    void generate_session_exponent();


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
    CryptoContext_mot(const ProtocolParameters & params, const cint &user_id, const cint &user_pk);
    /**
     * @brief hash1 Method makes hash1 function on user_id.
     * @param id_to_hash User Id to be hashed, it is of type mpz_class.
     * @return Output of hash function in type of mpz_class.
     */
    cint hash1(const cint &id_to_hash) const;

    /**
     * @brief ReadUserDataNotEncrypted Reads user info including his id and pk
     * @param user_data_file Name (path) to file containing data.
     * @param user_id Here will be put id read from file.
     * @param user_pk Here will be put pk (ltk) of user which is read from file.
     * @return True if success, false otherwise.
     */
    static bool ReadUserDataNotEncrypted(const char* user_data_filename, cint &user_id, cint &user_pk);

    cint protocol_message_cint();
    cint calculate_K(const cint &message, const cint &corespondent_id) const;



};

#endif // CRYPTOCONTEXT_MOT_H
