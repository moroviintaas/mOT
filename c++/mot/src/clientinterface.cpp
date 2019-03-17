#include "include/clientinterface.h"

ClientInterface::ClientInterface(const CryptoContext_mot &context, const std::string &address, uint16_t destination_port, uint16_t source_port)
{
    this->context = context;
    this->destination_port  = destination_port;
    this->source_port = source_port;
    this->address = address;


}

void ClientInterface::start()
{
    try {
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), address, std::to_string(destination_port));
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service, udp::endpoint(udp::v4(), source_port));
        //socket.open(udp::v4());

        boost::array<char, 1> send_buf  = { 0 };
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(
            boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
        std::cout<<"\n";


    } catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}
