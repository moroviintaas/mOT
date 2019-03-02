#include "include/cryptocontext_mot.h"
#include <openssl/sha.h>
/*CryptoContext_mot::CryptoContext_mot()
{

}*/

CryptoContext_mot::CryptoContext_mot(const SessionParameters &params)
{
    net_config = params;
}
