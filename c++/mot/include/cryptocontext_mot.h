#ifndef CRYPTOCONTEXT_MOT_H
#define CRYPTOCONTEXT_MOT_H
#include "include/protocolparameters.h"
//#include <boost/multiprecision/gmp.hpp>
#include <gmpxx.h>
#include "openssl/evp.h"
#include <random>
#include <ctime>
#define SIZE_UL sizeof(unsigned long)
#define BUFFSIZE 512



class CryptoContext_mot
{
    enum session_status{not_negotiated, running, expired};
protected:
    bool valid;
    ProtocolParameters net_config;
    std::string user_id;
    cint user_sk;
    std::string corresponder_id;
    uint8_t *negotiated_key;
    bool allocated_key;
    cint ephemeral_exponent;
    uint32_t session_id;
    session_status status;
    time_t last_active_timestamp;
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
    CryptoContext_mot(const ProtocolParameters & params, const std::string &user_id, const cint &user_pk);
    CryptoContext_mot(const CryptoContext_mot &context);
    ~CryptoContext_mot();
    /**
     * @brief hash1 Method makes hash1 function on user_id.
     * @param id_to_hash User Id to be hashed, it is of type mpz_class.
     * @return Output of hash function in type of mpz_class.
     */
    cint hash1(const std::string &id_to_hash) const;
    cint hash2(const cint &K_d, const std::string &id_source, uint32_t bytes_of_id_source, const std::string &id_dest, uint32_t bytes_of_id_dest, const cint& msg_source, const cint &msg_dest, uint32_t bytes_of_messages) const;
    /**
     * @brief ReadUserDataNotEncrypted Reads user info including his id and pk
     * @param user_data_file Name (path) to file containing data.
     * @param user_id Here will be put id read from file.
     * @param user_pk Here will be put pk (ltk) of user which is read from file.
     * @return True if success, false otherwise.
     */
    static bool ReadUserDataNotEncrypted(const char* user_data_filename, std::string &user_id, cint &user_sk);

    cint protocol_message_cint();
    cint calculate_K(const cint &message, const std::string &corespondent_id) const;
    /**
     * @brief copy_base Copies the context without ephemeral data.
     * @return Copy of context, does not set
     */
    CryptoContext_mot &operator=(const CryptoContext_mot &context);



    session_status get_status() const;
    time_t get_last_active_timestamp() const;
    void set_last_active_timestamp(const time_t &value);
    uint32_t get_session_id() const;
    void set_session_id(const uint32_t &value);

    uint16_t get_size_of_id_field() const;
    uint16_t get_size_of_initmsg_field() const;
    std::string get_user_id() const;
    std::string get_corresponder_id() const;
    void set_corresponder_id(const std::string &value);
};

#endif // CRYPTOCONTEXT_MOT_H
