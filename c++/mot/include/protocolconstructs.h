#ifndef PROTOCOL_FLAGS_H
#define PROTOCOL_FLAGS_H
#include <stdint.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "include/cryptocontext_mot.h"
//version

#define SETTING_1024_160_SHA256_SHA256 1

#define FIELD_VH 1
#define FIELD_VL 2
#define FIELD_SESSION_ID 3
#define FIELD_SETTING 7
#define FIELD_SIZE_SRC_ID 8
#define FIELD_SIZE_DEST_ID 10
#define FIELD_SIZE_HANDSHAKE_PAYLOAD 12
#define FIELD_MESSAGE_START 14

#define SIZE_OF_KGC_ID 4 //4 bytes of id

const uint8_t version_h = 0;
const uint8_t version_l = 1;

//Message types flag
const uint8_t flag_client_hello = 0x01;
const uint8_t flag_server_hello = 0x02;
const uint8_t flag_ping = 0x03;
const uint8_t flag_message = 0x04;
const uint8_t flag_termiante = 0x05;


void hello();

void uint64tobuffer(uint64_t number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
uint64_t buffertouint64(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point);
void uint32tobuffer(uint32_t number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
uint32_t buffertouint32(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point);

void uint16tobuffer(uint16_t number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
uint16_t buffertouint16(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point);
int32_t mpzclasstobuffer(const cint &number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
cint buffertompzclass(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point, size_t word_count);

int stringtobuffer(std::array<uint8_t, BUFFSIZE> & buffer, const std::string &s, size_t starting_point);
std::string buffertostring(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point, size_t num_of_bytes);


#endif // PROTOCOL_FLAGS_H
