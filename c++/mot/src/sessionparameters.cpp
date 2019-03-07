#include "include/sessionparameters.h"



SessionParameters::SessionParameters()
{

}

SessionParameters::SessionParameters(uint32_t rsa_key_size, uint32_t session_key_size, uint32_t ephemeral_exponent_size, uint32_t user_id_size,uint32_t h1_output_size, uint32_t h2_output_size)
{
    set_rsa_key_size(rsa_key_size);
    set_session_key_size(session_key_size);
    set_ephemeral_exponent_size(ephemeral_exponent_size);
    set_user_id_size(user_id_size);
    set_h1_output_size(h1_output_size);
    set_h2_output_size(h2_output_size);
}


uint32_t SessionParameters::get_rsa_key_size() const
{
    return rsa_key_size;
}

void SessionParameters::set_rsa_key_size(const uint32_t &value)
{
    rsa_key_size = value;
}

uint32_t SessionParameters::get_session_key_size() const
{
    return session_key_size;
}

void SessionParameters::set_session_key_size(const uint32_t &value)
{
    session_key_size = value;
}

uint32_t SessionParameters::get_ephemeral_exponent_size() const
{
    return ephemeral_exponent_size;
}

void SessionParameters::set_ephemeral_exponent_size(const uint32_t &value)
{
    ephemeral_exponent_size = value;
}

uint32_t SessionParameters::get_user_id_size() const
{
    return user_id_size;
}

void SessionParameters::set_user_id_size(const uint32_t &value)
{
    user_id_size = value;
}
uint32_t SessionParameters::get_h1_output_size() const
{
    return h1_output_size;
}

void SessionParameters::set_h1_output_size(const uint32_t &value)
{
    h1_output_size = value;
}

uint32_t SessionParameters::get_h2_output_size() const
{
    return h2_output_size;
}

void SessionParameters::set_h2_output_size(const uint32_t &value)
{
    h2_output_size = value;
}

SessionParameters &SessionParameters::operator=(const SessionParameters &params)
{
    set_rsa_key_size(params.get_rsa_key_size());
    set_session_key_size(params.get_session_key_size());
    set_ephemeral_exponent_size(params.get_ephemeral_exponent_size());
    set_user_id_size(params.get_user_id_size());
    set_h1_output_size(params.get_h1_output_size());
    set_h2_output_size(params.get_h2_output_size());
    return *this;
}

