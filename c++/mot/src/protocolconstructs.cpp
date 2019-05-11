#include "include/protocolconstructs.h"

void hello()
{
    std::cout<<"hello!\n";
}

void uint64tobuffer(uint64_t number, std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    for(size_t i=0; i<8; ++i)
    {
        buffer [starting_point+i] = uint8_t((number>>((8*i)))&0xff);
    }
}

uint64_t buffertouint64(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    uint64_t result = 0;
    for(size_t i=0; i<8; ++i)
    {
        result ^= (uint64_t(buffer[starting_point+i]&0xff)<<((8*i)));
        //buffer [starting_point+i] = uint8_t((number>>(24-(8*i)))&0xff);
    }
    return result;

}

void uint32tobuffer(uint32_t number, std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    for(size_t i=0; i<4; ++i)
    {
        buffer [starting_point+i] = uint8_t((number>>((8*i)))&0xff);
    }
}

uint32_t buffertouint32(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    uint32_t result = 0;
    for(size_t i=0; i<4; ++i)
    {
        result ^= uint32_t((buffer[starting_point+i]&0xff)<<((8*i)));
        //buffer [starting_point+i] = uint8_t((number>>(24-(8*i)))&0xff);
    }
    return result;

}
void uint16tobuffer(uint16_t number, std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    for(size_t i=0; i<2; ++i)
    {
        buffer [starting_point+i] = uint8_t((number>>((8*i)))&0xff);
    }
}
uint16_t buffertouint16(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    uint16_t result = 0;
    for(size_t i=0; i<2; ++i)
    {
        result ^= uint16_t((buffer[starting_point+i]&0xff)<<((8*i)));
        //buffer [starting_point+i] = uint8_t((number>>(24-(8*i)))&0xff);
    }
    return result;

}


int32_t mpzclasstobuffer(const cint &number, std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point)
{
    size_t pcount;
    mpz_export(buffer.data()+starting_point, &pcount, -1, sizeof (uint8_t), -1, 0, number.get_mpz_t());
    return 0;
}

cint buffertompzclass(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point, size_t word_count)
{
    cint result;
    mpz_import(result.get_mpz_t(), word_count, -1, sizeof (uint8_t), -1, 0, buffer.data()+starting_point);
    return result;
}


std::string buffertostring(std::array<uint8_t, BUFFSIZE> &buffer, size_t starting_point, size_t num_of_bytes)
{
    std::string result(num_of_bytes, 0);
    for (size_t i=0; i<num_of_bytes; ++i){
        result[i] = char(buffer[starting_point + i]);
    }
    return result;
}

int stringtobuffer(std::array<uint8_t, BUFFSIZE> &buffer, const std::string &s,size_t starting_point)
{
    if(starting_point + s.size()<= BUFFSIZE){
        for(size_t i=0 ;i < s.size(); ++i){
            buffer[starting_point+i] = uint8_t(s[i]);
        }
        return 0;
    }
    return -1;
}


