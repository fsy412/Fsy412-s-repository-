#include <dic.h>
#include <tinyxml.h>
#include <algorithm>   
namespace ghca{ 
 
int dictionary::scan(std::string path)
{
    DIR *pDir;  
    
    struct dirent *ent;  
    
    int i = 0;  
    
    pDir = opendir(path.c_str());  
   
    while((ent=readdir(pDir))!=NULL)  
    {  
        if(ent->d_type & DT_DIR)  
        {  
			continue;  
    	}  
		else
		{		
				TiXmlDocument doc ;
 				
				uint32_t vid;
				
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
				
				std::string name = path + "/" + ent->d_name;
				
				if (name.find(".xml") == std::string::npos) continue;
				
				if (!doc.LoadFile(name.c_str()))
				{
					std::cout << "load xml "<< name<<" failed!" << std::endl;
					
					return 1;
				}
							
				pbasic_dic_id_container = new basic_dic_id_container();
 
				if (pbasic_dic_id_container == NULL) 
				{
					std::cout << "allocate basic_dic_id_container failed"<<std::endl;
				}
				
				pbasic_dic_name_container = new basic_dic_name_container();
				
				if (pbasic_dic_name_container == NULL)
				{
					std::cout << "allocate basic_dic_name_container failed"<<std::endl;
				}
					
				TiXmlElement   *pcrElement=doc.RootElement();

				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
				
				std::string tmp =  ent->d_name;
 
				std::string vender = tmp.substr(0, tmp.find("."));
				
				std::string upper  = vender;
				
				std::transform(upper.begin(), upper.end(),upper.begin(), ::toupper);
				
				while(pElementtmp)
				{
					if ((strcmp(pElementtmp->Value(), vender.c_str() ) == 0 || strcmp(pElementtmp->Value(), upper.c_str() ) == 0 )&& pElementtmp->Attribute("vid") != NULL) 
					{ 
						vid = atoi(pElementtmp->Attribute("vid"));
					
					}else
					{
						vid = _basic_id_dic.size();
					}
 
					TiXmlElement* pchild = NULL;
		
					pchild = pElementtmp->FirstChildElement();
					
					while(pchild)
					{
 						basic_dic_avp avp;
						
						avp.id = atoi(pchild->Attribute("value"));
						
						if (strcmp(pchild->Attribute("type"), "integer") == 0)
						{
							avp.type = e_integer;
						
						}else if (strcmp(pchild->Attribute("type"), "string") == 0)
						{
							avp.type = e_string;
						
						}else if (strcmp(pchild->Attribute("type"), "ipaddr") == 0)
						{
							avp.type = e_ipaddr;
					
						}else if (strcmp(pchild->Attribute("type"), "octets") == 0)
						{
							avp.type = e_octets;
						}
  
						avp.name = pchild->Value();
						
						pbasic_dic_id_container->insert(std::make_pair(avp.id, avp));
						
						pbasic_dic_name_container->insert(std::make_pair(avp.name, avp));
						
						pchild = pchild ->NextSiblingElement();
					}
					
					pElementtmp=pElementtmp->NextSiblingElement();
				}
 
				_basic_dic_id_iter.push_back(pbasic_dic_id_container);
				
				_basic_id_dic.insert(std::make_pair(vid , _basic_dic_id_iter.size() - 1 ));
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair(vender, _basic_dic_name_iter.size() - 1));
		}
    }  
	return 0;
}

}
