#include <stdint.h>
#include <gmpxx.h>
#ifndef SESSIONPARAMETERS_H
#define SESSIONPARAMETERS_H
#include <openssl/sha.h>

//tmp include:
#include <iostream>
#include <iomanip>

typedef mpz_class cint;

class SessionParameters
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
     * @brief generator Group generator provided by KGC.
     */
    cint generator;
    /**
     * @brief h1_output_size Size of output of hash function H1 in bits.
     */
    uint32_t h1_output_size;
    /**
     * @brief h1_output_size Size of output of hash function H2 in bits.
     */
    uint32_t h2_output_size;
    /**
     * @brief kgc_modulus Modulus of KGC used in RSA based system of signing users ID.
     */
    cint kgc_modulus;
    /**
     * @brief kgc_public_exponent Public exponent of KGC (in RSA system).
     */
    cint kgc_public_exponent;



public:

    SessionParameters();
    SessionParameters(uint32_t rsa_key_size, uint32_t session_key_size, uint32_t ephemeral_exponent_size, uint32_t user_id_size, uint32_t h1_output_size, uint32_t h2_output_size);
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

    /**
     * @brief operator = is standard assignment operator.
     * @param params is assigned object of type SessionParams
     * @return is reference to object to which values has been assigned.
     */
    SessionParameters & operator=(const SessionParameters & params);

};

#endif // SESSIONPARAMETERS_H
