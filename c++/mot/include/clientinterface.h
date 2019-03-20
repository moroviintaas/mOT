#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include "cryptocontext_mot.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "include/protocolconstructs.h"
#include <vector>

using boost::asio::ip::udp;
class ClientInterface
{
protected:
    //CryptoContext_mot context;
    uint16_t source_port;
    uint16_t destination_port;
    std::string address;
    //sockaddr_in

public:
    ClientInterface( const std::string &address, uint16_t destination_port, uint16_t source_port);
    void start(const CryptoContext_mot &context);
};

#endif // CLIENTINTERFACE_H
