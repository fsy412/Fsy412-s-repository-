#include <radiusdecode.h>
#include <iostream>

namespace ghca{

	int radiusdecode::basic_scan(const uint8_t* packet,uint8_t len)
	{
		uint8_t* p =(uint8_t*) packet;

		if (p == NULL)
		{
			strcpy(_errormsg,"the scan package is null.");

			return -1;
		}

		if(len <= 20)
		{
			strcpy(_errormsg,"the scan package length is less than 20.");

			return -1;
		}

		if(get_packet_len(p)!=len)
		{
			strcpy(_errormsg,"the scan package length is incorrect.");

			return -1;
		}

		p += HEADER;

		while(len > HEADER)
		{
			int ret;

			if(p[0] != 26)
			{
				ret = basic_scan_avp(p);
			}
			else
			{
				ret = basic_scan_specific_avp(p);
			}

			if(ret ==-1)
			{
				return -1;
			}
			
			len -= ret;

			p += ret;
		}

		return 0;
	}

	int radiusdecode::get_packet_len(uint8_t* packet)
	{
		return(packet[2] * 256 + packet[3]) ;
	}

	int radiusdecode::get_avp_len(uint8_t* avp)
	{
		return (avp[1]);
	}

	int radiusdecode::basic_scan_avp(uint8_t* avp)
	{
		basic_radius_avp avps;

		avps.id = avp[0];

		avps.len = avp[1];

		if(avps.len <= 2)
		{
			strcpy(_errormsg,"the avp length is less than 2.");

			return -1;
		}

		memcpy(avps.avp,avp+2,avps.len-2);

		_basic_container.insert(std::make_pair(avps.id,avps));

		return avps.len;
	}

	int radiusdecode::basic_scan_specific_avp(uint8_t* avp)
	{
		basic_specific_avp avps;

		uint8_t pos = 0;

		uint8_t len = avp[1];

		if(len <=6)
		{
			strcpy(_errormsg,"the specific avp length is less than 6.");

			return -1;
		}

		uint32_t vid = avp[2] * 4294967296 + avp[3] * 16777216 + avp[4] * 65536 + avp[5];

		avp += 6;

		while (len > 6)
		{
			
			avps.vid = vid;

			avps.type = avp[pos];

			avps.len = avp[1];

			if(avps.len <= 2)
			{
				strcpy(_errormsg,"the attribute specific length is less than 2.");

				return -1;
			}

			memcpy(avps.avp,avp+2,avps.len - 2);

			avp += avps.len;

			len -= avps.len;

			_basic_specific_container.insert(std::make_pair(avps.vid,avps));

		}
		
		return len;
	}

	int radiusdecode::get_avp(uint8_t id,basic_radius_avp& avp)
	{
		basic_avp_container::iterator iter;

		if((iter = _basic_container.find(id)) != _basic_container.end())
		{
			memcpy((void*)&avp,(void*)&(iter->second),sizeof(basic_radius_avp));

			return 0;
		}
		else
		{
			return -1;
		}		
	}

	int radiusdecode::get_specific_avp(uint32_t vid,uint8_t type,basic_specific_avp& avp)
	{
		std::pair <basic_specific_avp_container::iterator, basic_specific_avp_container::iterator> ret;

		ret = _basic_specific_container.equal_range(vid);

		for(basic_specific_avp_container::iterator iter = ret.first;iter != ret.second;++iter)
		{
			if (iter->second.type ==  type)
			{
				memcpy((void*)&avp,(void*)&(iter->second),sizeof(basic_specific_avp));

				return 0;
			}
		}

		return -1;
	}


	char* radiusdecode::get_err_msg()
	{
		return _errormsg;
	}
}