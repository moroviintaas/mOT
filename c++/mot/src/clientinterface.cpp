#include "include/clientinterface.h"

ClientInterface::ClientInterface(const std::string &address, uint16_t destination_port, uint16_t source_port)
{
    //this->context = context;
    this->destination_port  = destination_port;
    this->source_port = source_port;
    this->address = address;


}

void ClientInterface::start(const CryptoContext_mot &context)
{
    CryptoContext_mot tmp_context(context);
    std::random_device rd;
    std::uniform_int_distribution<uint32_t> d(0, UINT16_MAX);
    std::ifstream rin("/dev/urandom");
    uint32_t session_id;
    uint16_t size_in_mod = context.get_size_of_initmsg_field();
    uint16_t valid_size_of_id_source = context.get_size_of_id_field();
    uint16_t size_of_id_dest =0;
    uint16_t received_size_of_id_source;
    std::string corresponder_id;
    CryptoContext_mot actual_context = context;
    cint client_message;
    cint server_message;
    cint K,K2;


    //std::cout<<"id:\t"<<context.get_user_id()<<"\n";

    //uint8_t *send_buf  = new uint8_t[BUFFSIZE];
    //uint8_t *recv_buf  = new uint8_t[BUFFSIZE];
    boost::system::error_code ignored_error;
    std::array<uint8_t, BUFFSIZE> send_buf  = { 0 };
    try {
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), address, std::to_string(destination_port));
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service, udp::endpoint(udp::v4(), source_port));
        std::array<uint8_t, BUFFSIZE> recv_buf;
        udp::endpoint sender_endpoint;
        uint16_t size_of_src_id = uint16_t(context.get_user_id().size());
        //socket.open(udp::v4());

        session_id = d(rd);

        client_message = tmp_context.protocol_message_cint();



        send_buf.fill(0);
        send_buf[0] = flag_client_hello;
        send_buf[FIELD_VH] = version_h;//version 0.1
        send_buf[FIELD_VL] = version_l;

        uint32tobuffer(session_id, send_buf, FIELD_SESSION_ID);

        // Hardcoded setting for now:
        send_buf[FIELD_SETTING] = SETTING_1024_160_SHA256_SHA256;

        uint16tobuffer(size_of_src_id, send_buf, FIELD_SIZE_SRC_ID);
        uint16tobuffer(size_of_id_dest ,send_buf,FIELD_SIZE_DEST_ID);
        uint16tobuffer(size_in_mod, send_buf, FIELD_SIZE_HANDSHAKE_PAYLOAD);
        stringtobuffer( send_buf,context.get_user_id(),FIELD_MESSAGE_START);
        mpzclasstobuffer(client_message,send_buf,FIELD_MESSAGE_START+size_of_id_dest+size_of_src_id);
        send_buf[FIELD_MESSAGE_START+size_of_id_dest+size_of_src_id+size_in_mod] = SIZE_OF_KGC_ID;
        //std::cout<<std::hex<<context.get_kgc_id()<<"\n";
        uint64tobuffer(context.get_kgc_id(), send_buf, FIELD_MESSAGE_START+size_of_id_dest+size_of_src_id+size_in_mod+1 );





        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint,0,ignored_error);


        size_t len = socket.receive_from(
            boost::asio::buffer(recv_buf), sender_endpoint);

        if(recv_buf[0] == flag_client_hello)
        {
            //std::cout<<"old:"<<std::hex<<session_id<<"\n";
            //session_id = uint32_t((recv_buf[3]&0xff)<<24) ^ uint32_t((recv_buf[4]&0xff)<<16) ^ uint32_t((recv_buf[5]&0xff)<<8)  ^ uint32_t((recv_buf[6]&0xff));
            session_id = buffertouint32(recv_buf, FIELD_SESSION_ID);
            size_of_id_dest = buffertouint16(recv_buf,FIELD_SIZE_SRC_ID);
            received_size_of_id_source = buffertouint16(recv_buf,FIELD_SIZE_DEST_ID);
            corresponder_id = buffertostring(recv_buf,FIELD_MESSAGE_START,size_of_id_dest);
            //received_size_of_id_source = buffertouint16(recv_buf,FIELD_SIZE_DEST_ID);
            if (size_in_mod != buffertouint16(recv_buf,FIELD_SIZE_HANDSHAKE_PAYLOAD))
            {
                std::cout<<"not correct mod size!\n";
            }

            //std::cout<<size_of_id_dest<<" "<<int(recv_buf[13]);
            //std::cout<<"corresponder id:\t"<<corresponder_id<<"\n";
            server_message = buffertompzclass(recv_buf,FIELD_MESSAGE_START+size_of_id_dest + received_size_of_id_source,size_in_mod);
            K = tmp_context.calculate_K(server_message,corresponder_id);
            K2 = tmp_context.hash2(K,tmp_context.get_user_id(),size_of_src_id, corresponder_id, size_of_id_dest, client_message, server_message, size_in_mod);

            tmp_context.set_corresponder_id(corresponder_id);
            tmp_context.set_session_id(session_id);
            /*
            std::cout<<"client message:\t"<<client_message<<"\n";
            cint serv_msg (server_message);
            std::cout<<"server message:'t"<<serv_msg<<"\n";
            */
            /*
            std::cout<<"sessionid:\t"<<std::hex<<session_id<<"\n";
            std::cout<<"my id:\t"<<std::hex<<context.get_user_id()<<"\n";
            std::cout<<"corresponder id\t:"<<std::hex<<corresponder_id<<"\n";
            std::cout<<"client message:\t"<<client_message<<"\n";
            std::cout<<"server messgae:\t"<<server_message<<"\n";
            std::cout<<"ss=\t"<<K<<"\n";*/

            std::cout<<"K=\t"<<std::hex<<K2<<"\n";

            send_buf[0] = flag_termiante;
            uint32tobuffer(session_id,send_buf,3);
            socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint,0,ignored_error);

        }
        else {
            //std::cout.write(recv_buf.data(), len);
            std::cout<<"\n";
        }





    } catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}
