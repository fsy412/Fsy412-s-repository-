#include <iostream>
#include <map>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define MIN_RADIUS_LEN		20
#define MAX_RADIUS_LEN		4096
#define AUTH_VECTOR_LEN		16

#define AUTH_HDR_LEN		20
#define MAX_PACKET_CODE 	13

typedef struct radius_packet_t {
  uint8_t	code;
  uint8_t	id;
  uint8_t	length[2];
  uint8_t	vector[AUTH_VECTOR_LEN];
  uint8_t	data[1];
} radius_packet_t;


typedef std::map<std::string,  std::string> RADIUS_PACKET_MAP;
RADIUS_PACKET_MAP radius_packet_map;
char packet []= {0x01,0x02,0x00,0x31,0x20,0x20,0x20,0x20,0x20,0x20,0x31,0x33,0x38,0x37,0x33,0x33,0x36,0x30,0x35,0x34,0x01,0x06,0x74,0x65,0x73,0x74,0x02,0x12,0x86,0x10,0xbd,0x5f,0xdb,0x71,0x09,0xe7,0xd8,0xba,0xbc,0xef,0x29,0xe9,0x3b,0x09,0x01,0x05,0x31,0x32,0x33};
 
int save_radius_packet( char* pack ,unsigned length, RADIUS_PACKET_MAP& radius_packet_map)
{	
	unsigned int num_attributes;
	unsigned int total_len;
	unsigned int packet_len;
	unsigned int count;
	unsigned char *attr;
	std::string key;
 
	total_len = (pack[2] << 8) | pack[3];
 	std::cout<< "total_len "<<total_len<<std::endl;
	radius_packet_t* radius_packet = (radius_packet_t*)pack;
	if (total_len < MIN_RADIUS_LEN || total_len > MAX_RADIUS_LEN)
	{
		std::cout<< "Invalid RADIUS packet len is invalid"<<std::endl;
		return 1;
	}
		
	if (total_len != length)
	{
		std::cout<< "Invalid RADIUS packet len is mismatch "<<std::endl;
		return 1;
	}
	std::cout<< "radius_packet->code  "<<radius_packet->code<<std::endl;
	printf("radius_packet->code %x\n", radius_packet->code);
	printf("pack[0] %x\n", pack[0]);

 	if ((radius_packet->code == 0 ) || radius_packet->code >=  MAX_PACKET_CODE )
	{
	 	std::cout<< "Invalid RADIUS packet code is invalid "<<std::endl;
		return 1;
	}
	char buf[10];
	sprintf(buf, "%d", radius_packet->id);
	key = buf;
	std::cout<<"key:"<<key<<std::endl;
	 
	attr = radius_packet->data;
	count = total_len - AUTH_HDR_LEN;
	num_attributes = 0;
	
	while (count > 0)
	{
		if (count < 2)
		{
			std::cout<< "Invalid RADIUS packet attribute header is invalid "<<std::endl;
			return 1;
		}
		
	 
		if (attr[0] == 0)
		{
 			std::cout<< "Invalid RADIUS packet Attribute number zero is NOT defined. "<<std::endl;
			return 1;
		}
 
       	if (attr[1] < 2)
		{
			std::cout<< "Invalid RADIUS packet attribute is invalid "<<std::endl;
			return 1;
		}
 
		if (count < attr[1])
		{
			std::cout<< "Invalid RADIUS packet attribute is invalid "<<std::endl;
			return 1;
		}
		
		switch (attr[0])
		{
 			case 26:
				{
					int vendor_id;
					vendor_id = (attr[3] << 16) | (attr[4] << 8) | attr[5];
 					char buf[10];
					sprintf(buf, "%d", vendor_id);
					key = key + buf;
				}
				break;
			default:	 
				break;
 
		}
		
		count -= attr[1];	 
		attr += attr[1];
		num_attributes++;	 
	}
	
	if (count != 0)
	{	 
		std::cout<< "Invalid RADIUS packet "<<std::endl;
		return 1;
	}
	
	//radius_data[key] = data;
	
	return 0;
}

std::string  radius_msg(std::string  input)
{
	int j = 0 ;
	char *ret = new char[input.size() / 2  + 1];
	std::string str;
	for ( int i = 0 ; i < input.size() / 2; i ++)
	{
		std::string sub = input.substr(i * 2 , 2);
		unsigned int x;
 		sscanf(sub.c_str(),"%x", &x);
		memcpy(ret + i, &x, 1 );
  		char c ;
		memcpy(&c ,&x,1);
		str= str + c;
 
	}
	ret[input.size() / 2  + 1] = '\0';
 
	
 return str;
}
int main()
{
	char *str = "010200312020202020203133383733333630353401067465737402128610bd5fdb7109e7d8babcef29e93b090105313233";
	char packet []= {0x01,0x02,0x00,0x31,0x20,0x20,0x20,0x20,0x20,0x20,0x31,0x33,0x38,0x37,0x33,0x33,0x36,0x30,0x35,0x34,0x01,0x06,0x74,0x65,0x73,0x74,0x02,0x12,0x86,0x10,0xbd,0x5f,0xdb,0x71,0x09,0xe7,0xd8,0xba,0xbc,0xef,0x29,0xe9,0x3b,0x09,0x01,0x05,0x31,0x32,0x33};
	//std::cout<<"main "<< packet[28]<<std::endl;
	//printf("main:%d %#x\n", packet[28], packet[28]);
	
	std::string redismsg = radius_msg(str);
 	save_radius_packet((char*)redismsg.c_str() , 49, radius_packet_map);
	//save_radius_packet(packet ,radius_packet_map);
	return 0;
}