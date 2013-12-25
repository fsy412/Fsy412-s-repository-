#ifndef DIC
#define DIC

#include <map>
#include <vector>
#include <basic_radius_package.h>


#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>  
#include <dirent.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <sys/types.h>
#include <iostream>
#include <tinyxml.h>


namespace ghca{	

	typedef std::map<uint8_t,uint8_t> basic_id_dic;

	typedef std::map<uint8_t,basic_dic_avp> basic_dic_id_container;

	typedef std::vector<basic_dic_id_container*> basic_dic_id_iter;

	typedef std::map<std::string,uint8_t> basic_name_dic;

	typedef std::map<std::string,basic_dic_avp> basic_dic_name_container;

	typedef std::vector<basic_dic_name_container*> basic_dic_name_iter;
	enum 
	{
		e_integer = 0,
		e_string,
		e_ipaddr, 
		e_octets
	};
	
	class dictionary{

	public:

		dictionary(){};

		~dictionary(){};

	public:
			
		int scan(std::string path);
		
 
		

	private:

		basic_name_dic _basic_name_dic;

		basic_id_dic _basic_id_dic;

		basic_dic_id_container _basic_dic_id_container;

		basic_dic_id_iter _basic_dic_id_iter;

		basic_dic_name_container _basic_dic_name_container;

		basic_dic_name_iter _basic_dic_name_iter;
	};
}


#endif