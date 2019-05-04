#include "include/serverinterface.h"


ServerInterface::ServerInterface(uint16_t listening_port)
{

    //this->context = context;
    this->listening_port = listening_port;


}

void ServerInterface::start(const CryptoContext_mot &context_template)
{

    std::random_device rd;
    std::uniform_int_distribution<uint32_t> d(0, UINT16_MAX);
    std::ifstream rin("/dev/urandom");
    uint32_t new_session_id;
    uint32_t operating_session_id;
    cint client_message;
    cint server_message;
    cint K,K2;

    //uint8_t *send_buf  = new uint8_t[BUFFSIZE];
    //uint8_t *recv_buf  = new uint8_t[BUFFSIZE];
    try
    {
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), listening_port));


        std::array<uint8_t, BUFFSIZE> recv_buf;
        udp::endpoint remote_endpoint;
        boost::system::error_code error;
        uint16_t size_in_mod = context_template.get_size_of_initmsg_field();
        uint16_t valid_size_of_id_source = context_template.get_size_of_id_field();
        uint16_t size_of_src_id = uint16_t(context_template.get_user_id().size());
        uint16_t received_size_of_id_source;
        uint16_t size_of_id_dest;
        std::string corresponder_id;
        CryptoContext_mot tmp_context(context_template);
        std::array<uint8_t, BUFFSIZE> send_buf = {0};


        for (;;)
        {

            socket.receive_from(boost::asio::buffer(recv_buf),remote_endpoint, 0, error);

            //socket.receive_from(recv_buf,remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size)
              throw boost::system::system_error(error);

            if(recv_buf[0]==flag_client_hello)
            {
                std::cout<<"Requested new session\n";

                //std::cout<<"buffer:\n";

                size_of_id_dest = buffertouint16(recv_buf,7);
                received_size_of_id_source = buffertouint16(recv_buf,9);
                if (size_in_mod != buffertouint16(recv_buf,11))
                {
                    std::cout<<"not correct mod size!\n";
                }
                corresponder_id = buffertostring(recv_buf, 13,size_of_id_dest);
                client_message = buffertompzclass(recv_buf,MESSAGE_START+size_of_id_dest+received_size_of_id_source,size_in_mod);

                server_message = tmp_context.protocol_message_cint();


                new_session_id = d(rd)<<16;
                new_session_id ^= buffertouint32(recv_buf,3);

                K = tmp_context.calculate_K(client_message,corresponder_id);
                K2 = tmp_context.hash2(K, corresponder_id, size_of_id_dest,tmp_context.get_user_id(),size_of_src_id, client_message, server_message, size_in_mod);
                //std::cout<<std::hex<<buffertouint32(recv_buf,3)<<"\n";
                //new_session_id ^= ((uint32_t(recv_buf[3])<<24) ^ uint32_t(recv_buf[4])<<16);

                //std::cout<<std::dec<<size_of_id_dest<<"\t"<<context_template.get_user_id()<<"\n";
                send_buf.fill(0);
                send_buf [0] = flag_client_hello;
                send_buf [1] = version_h;
                send_buf [2] = version_l;
                /*
                send_buf [3] = uint8_t((new_session_id>>24)&0xff);
                send_buf [4] = uint8_t((new_session_id>>16)&0xff);
                send_buf [5] = uint8_t((new_session_id>>8)&0xff);
                send_buf [6] = uint8_t((new_session_id)&0xff);
                */

                uint32tobuffer(new_session_id, send_buf,3);
                uint16tobuffer(size_of_src_id,send_buf,7);
                uint16tobuffer(size_of_id_dest,send_buf,9);
                uint16tobuffer(size_in_mod,send_buf,11);
                //memset(send_buf.data()+MESSAGE_START, 0, MESSAGE_START+size_of_src_id + size_of_id_dest + size_in_mod);
                stringtobuffer(send_buf,context_template.get_user_id(),MESSAGE_START);
                stringtobuffer(send_buf, corresponder_id,MESSAGE_START+size_of_src_id);
                mpzclasstobuffer(server_message,send_buf,MESSAGE_START+size_of_src_id + size_of_id_dest);
                /*
                std::cout<<"client message:\t"<<server_message<<"\n";
                cint clnt_msg (client_message);
                std::cout<<"server message:'t"<<clnt_msg<<"\n";
                */
                /*
                std::cout<<"sessionid:\t"<<std::hex<<new_session_id<<"\n";
                std::cout<<"my id:\t"<<std::hex<<context_template.get_user_id()<<"\n";
                std::cout<<"corresponder id\t:"<<std::hex<<corresponder_id<<"\n";
                std::cout<<"client message:\t"<<client_message<<"\n";
                std::cout<<"server messgae:\t"<<server_message<<"\n";
                std::cout<<"ss=\t"<<K<<"\n";*/

                std::cout<<"K=\t"<<K2<<"\n";

                tmp_context.set_corresponder_id(corresponder_id);
                tmp_context.set_session_id(new_session_id);

                vec_context.push_back(CryptoContext_mot(tmp_context));
                boost::system::error_code ignored_error;
                socket.send_to(boost::asio::buffer(send_buf),                    remote_endpoint, 0, ignored_error);
                //socket.send_to(send_buf, remote_endpoint, 0, ignored_error);
            }

            else if (recv_buf[0]==flag_termiante)
            {
                operating_session_id = buffertouint32(recv_buf,3);
                std::cout<<"closing session:"<<std::hex<<operating_session_id<< "\n";
                //for(size_t i =0; i< vec_context.size(); ++i)
                std::vector<CryptoContext_mot>::iterator tmp;
                for(size_t i =0; i<vec_context.size();  ++i)
                {

                    if(vec_context[i].get_session_id() == operating_session_id)
                    {
                        vec_context.erase(vec_context.begin()+i);
                    }
                }
            }

            else
            {
                std::string message = "Hello";

                boost::system::error_code ignored_error;
                socket.send_to(boost::asio::buffer(message),
                    remote_endpoint, 0, ignored_error);
            }
            std::cout<<"active sessions:\t"<<std::dec<<vec_context.size()<<"\n";

        }
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }


}
