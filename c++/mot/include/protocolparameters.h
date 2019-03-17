#include <stdint.h>
#include <gmpxx.h>
#ifndef SESSIONPARAMETERS_H
#define SESSIONPARAMETERS_H
#include <openssl/sha.h>
#include <fstream>
#include <string>

//tmp include:
#include <iostream>
#include <iomanip>

#define MIN_RSA_KEY_SIZE 1024
#define MIN_SESSION_KEY_SIZE 128
#define MIN_EPHEMERAL_EXPONENT_SIZE 64
#define MIN_USER_ID_SIZE  16
#define MIN_H1_OUTPUT_SIZE 128
#define MIN_H2_OUTPUT_SIZE 128


typedef mpz_class cint;

class ProtocolParameters
{
protected:
    /**
     * @brief rsa_key_size The size in bits of RSA module used in session, note that this is size used in communication and storage.
     */
    uint32_t rsa_key_size;
    /**
     * @brief session_key_size Size in bits of negotiated shared_secred (used directly as session key or seed for it).
     */
    uint32_t session_key_size;
    /**
     * @brief ephemeral_exponent_size The size in bits of short-term exponent used in key negotiation.
     * There is no practical point of this to be larger than modulus (rsa_key_size).
     */
    uint32_t ephemeral_exponent_size;
    /**
     * @brief user_id_size The size in bits of user identificator in the system. Note that all users id must not exceed this size.
     */
    uint32_t user_id_size;

    /**
     * @brief h1_hash_size Size of output of hash function H1 in bits (before squaring mod N).
     */
    uint32_t h1_output_size;
    /**
     * @brief h1_hash_size Size of output of hash function H2 in bits.
     */
    uint32_t h2_output_size;
    /**
     * @brief generator Group generator provided by KGC.
     */
    cint generator;
    /**
     * @brief kgc_modulus Modulus of KGC used in RSA based system of signing users ID.
     */
    cint kgc_modulus;
    /**
     * @brief kgc_public_exponent Public exponent of KGC (in RSA system).
     */
    cint kgc_public_exponent;
    bool valid;



public:

    ProtocolParameters();
    ProtocolParameters(uint32_t rsa_key_size, uint32_t session_key_size, uint32_t ephemeral_exponent_size, uint32_t user_id_size, uint32_t h1_hash_size, uint32_t h2_hash_size, const cint & kgc_modulus, const cint & kgc_public_exponent, const cint & generator);
    uint32_t get_rsa_key_size() const;
    void set_rsa_key_size(const uint32_t &value);
    uint32_t get_session_key_size() const;
    void set_session_key_size(const uint32_t &value);
    uint32_t get_ephemeral_exponent_size() const;
    void set_ephemeral_exponent_size(const uint32_t &value);
    uint32_t get_user_id_size() const;
    void set_user_id_size(const uint32_t &value);
    uint32_t get_h1_output_size() const;
    void set_h1_output_size(const uint32_t &value);
    uint32_t get_h2_output_size() const;
    void set_h2_output_size(const uint32_t &value);
    cint get_generator() const;
    void set_generator(const cint &value);
    cint get_kgc_public_exponent() const;
    void set_kgc_public_exponent(const cint &value);
    cint get_kgc_modulus() const;
    void set_kgc_modulus(const cint &value);
    bool get_valid() const;

    /**
     * @brief operator = is standard assignment operator.
     * @param params is assigned object of type SessionParams
     * @return is reference to object to which values has been assigned.
     */
    ProtocolParameters & operator=(const ProtocolParameters & params);

    static bool ReadProtocolParameters(const char* config_file_name, uint32_t &rsa_key_size, uint32_t &session_key_size, uint32_t &ephemeral_exponent_size, uint32_t &user_id_size, uint32_t &h1_hash_size, uint32_t &h2_hash_size,  cint & kgc_modulus, cint & kgc_public_exponent, cint & generator);




};

#endif // SESSIONPARAMETERS_H
