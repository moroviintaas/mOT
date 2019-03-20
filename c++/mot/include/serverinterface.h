#ifdef __linux__

#include "cryptocontext_mot.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <vector>
#include "include/protocolconstructs.h"

#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H


using boost::asio::ip::udp;
class ServerInterface
{

protected:
    std::vector<CryptoContext_mot> vec_context;
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
    ServerInterface(uint16_t listening_port);
    void start(const CryptoContext_mot &context_template);
};


#endif // NETWORKINTERFACE_H
#endif // __linux__
