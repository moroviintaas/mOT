#include "include/serverinterface.h"


ServerInterface::ServerInterface(const CryptoContext_mot &context, uint16_t listening_port)
{

    this->context = context;
    this->listening_port = listening_port;


}

void ServerInterface::start()
{
    try
    {
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), listening_port));

        for (;;)
        {
            boost::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;
            boost::system::error_code error;
            socket.receive_from(boost::asio::buffer(recv_buf),
                remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size)
              throw boost::system::system_error(error);

            std::string message = "Hello";

            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message),
                remote_endpoint, 0, ignored_error);
        }
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }


}
