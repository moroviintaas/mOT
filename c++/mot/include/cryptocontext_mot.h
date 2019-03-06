#ifndef CRYPTOCONTEXT_MOT_H
#define CRYPTOCONTEXT_MOT_H
#include "include/sessionparameters.h"
//#include <boost/multiprecision/gmp.hpp>
#include <gmpxx.h>


typedef mpz_class cint;

class CryptoContext_mot
{
protected:
    SessionParameters net_config;
public:
    //CryptoContext_mot();
    CryptoContext_mot(const SessionParameters & params);
};

#endif // CRYPTOCONTEXT_MOT_H
