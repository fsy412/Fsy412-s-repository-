#ifndef RADIUSDECODE
#define RADIUSDECODE
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <vector>
#include <basic_radius_package.h>

namespace ghca{

	typedef std::map<uint8_t,basic_radius_avp> basic_avp_container;

	typedef std::multimap<uint8_t,basic_specific_avp> basic_specific_avp_container;

	





	class radiusdecode{

	public:

		radiusdecode(){};

		~radiusdecode(){

			_basic_container.clear();

			_basic_specific_container.clear();
		};

	public:

		int basic_scan(const uint8_t* packet,uint8_t len);

		int get_avp(uint8_t id,basic_radius_avp& avp);

		int get_specific_avp(uint32_t vid,uint8_t type,basic_specific_avp& avp);

		char* get_err_msg();

	private:

		int get_packet_len(uint8_t* packet);

		int get_avp_len(uint8_t* avp);

		int basic_scan_avp(uint8_t* avp);

		int basic_scan_specific_avp(uint8_t* avp);


	private:

		basic_avp_container _basic_container;

		basic_specific_avp_container _basic_specific_container;

		char _errormsg[255];


	};

}


#endif