#ifndef BASIC_RADIUS_PACKAGE
#define BASIC_RADIUS_PACKAGE


#include <stdint.h>
#include <string>
#include <inttype.h>



#define  HEADER 20

namespace ghca{

	typedef struct radius_header{
		
		uint8_t code;

		uint8_t id;

		uint16_t len;

		uint32_t auth;

	};

	typedef struct basic_radius_avp{

		uint8_t id;

		uint8_t len;

		uint8_t avp[255];

	};


	typedef struct basic_specific_avp{

		uint32_t vid;

		uint8_t len;

		uint8_t type;

		uint8_t avp[255];

	};

	typedef struct basic_dic_avp{

		uint8_t id;

		uint8_t type;

		std::string name;

	};
}
#endif