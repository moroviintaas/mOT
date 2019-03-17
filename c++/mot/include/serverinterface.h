#ifdef __linux__

#include "cryptocontext_mot.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>

#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H


using boost::asio::ip::udp;
class ServerInterface
{

protected:
    CryptoContext_mot context;
    uint16_t listening_port;
    //sockaddr_in s_address;

    /*
    int server_file_descriptor;
    int new_socket;
    int valread;
    char buffer[BUFFSIZE] = {0};
    int opt;
    int addr_len;
    */

public:
    ServerInterface(const CryptoContext_mot &context, uint16_t listening_port);
    void start();
};


#endif // NETWORKINTERFACE_H
#endif // __linux__
