#ifndef PROTOCOL_FLAGS_H
#define PROTOCOL_FLAGS_H
#include <stdint.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "include/cryptocontext_mot.h"
//version

#define MESSAGE_START 13

const uint8_t version_h = 0;
const uint8_t version_l = 1;

//Message types flag
const uint8_t flag_client_hello = 0x01;
const uint8_t flag_server_hello = 0x02;
const uint8_t flag_ping = 0x03;
const uint8_t flag_message = 0x04;
const uint8_t flag_termiante = 0x05;


void hello();

void uint32tobuffer(uint32_t number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
uint32_t buffertouint32(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point);
void uint16tobuffer(uint16_t number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
uint16_t buffertouint16(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point);
int32_t mpzclasstobuffer(const cint &number, std::array<uint8_t, BUFFSIZE> & buffer, size_t starting_point);
cint buffertompzclass(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point, size_t word_count);

#endif // PROTOCOL_FLAGS_H
