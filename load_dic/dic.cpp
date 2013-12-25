#include <dic.h>
#include <tinyxml.h>
   
 namespace ghca{ 
 
int dictionary::scan(std::string path)
{
	DIR              *pDir ;  
    struct dirent    *ent  ;  
    int               i=0  ;  
    char              childpath[512];  
 
    pDir=opendir(path.c_str());  
	
    memset(childpath,0,sizeof(childpath));  
 
  
    while((ent=readdir(pDir))!=NULL)  
    {  
        if(ent->d_type & DT_DIR)  
        {  
			continue;  
    	}  
		else
		{
 			if (std::string (ent->d_name ) == "rfc.xml")
			{
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
				
				std::string name = path + "/rfc.xml";
				
				TiXmlDocument doc ;
				
				if (!doc.LoadFile(name.c_str()))
				{
					std::cout << "load xml failed!" << std::endl;
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

				while(pElementtmp)
				{
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
				
				_basic_id_dic.insert(std::make_pair(_basic_id_dic.size(), _basic_dic_id_iter.size() - 1 ));
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("rfc", _basic_dic_name_iter.size() - 1));
 
			}else if (std::string (ent->d_name ) == "huawei.xml")
			{
				uint8_t key;
 
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
				
				std::string name = path + "/huawei.xml";
				
				TiXmlDocument doc ;
 
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
				
				TiXmlElement  *pcrElement=doc.RootElement();
				
				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
				
 				while(pElementtmp)
				{
					if (strcmp(pElementtmp->Value(),"HUAWEI") == 0) 
					{
						key = atoi(pElementtmp->Attribute("vid"));
					}

					TiXmlElement* pchild=NULL;

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
				
				_basic_id_dic.insert(std::make_pair(key, _basic_dic_id_iter.size() - 1 ));	
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("HUAWEI", _basic_dic_name_iter.size() - 1));
			
			}else if (std::string (ent->d_name ) == "cisco.xml")
			{
				uint8_t key;
				
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
				
	 			std::string name = path + "/cisco.xml";
				
				TiXmlDocument doc ;
				
				if (!doc.LoadFile(name.c_str()))
				{
					std::cout << "load xml failed!" << std::endl;
					
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
				
				TiXmlElement  *pcrElement=doc.RootElement();
				
				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
				
	 			while(pElementtmp)
				{
					if (strcmp(pElementtmp->Value(),"cisco") == 0) 
					{
						key = atoi(pElementtmp->Attribute("vid"));
 					}
					
					TiXmlElement* pchild=NULL;
					
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
				
				_basic_id_dic.insert(std::make_pair(key, _basic_dic_id_iter.size() - 1));	
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("cisco", _basic_dic_name_iter.size() - 1));
			
			}else if (std::string (ent->d_name ) == "redback.xml")
			{
				uint8_t key;
				
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
	 		
				std::string name = path + "/redback.xml";
				
				TiXmlDocument doc ;
				
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
				
				TiXmlElement  *pcrElement=doc.RootElement();
				
				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
				
	 			while(pElementtmp)
				{
					
					if (strcmp(pElementtmp->Value(),"redback") == 0) 
					{
						key = atoi(pElementtmp->Attribute("vid"));
 					}
					
					TiXmlElement* pchild=NULL;
					
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
				
				_basic_id_dic.insert(std::make_pair(key, _basic_dic_id_iter.size() - 1));	
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("redback", _basic_dic_name_iter.size() - 1));
			
			}else if (std::string (ent->d_name ) == "udb.xml")
			{
				uint8_t key;
				
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
	 		
				std::string name = path + "/udb.xml";
				
				TiXmlDocument doc ;
				
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
				
				TiXmlElement  *pcrElement=doc.RootElement();
				
				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
 
	 			while(pElementtmp)
				{
					
					if (strcmp(pElementtmp->Value(),"udb") == 0) 
					{
						key = atoi(pElementtmp->Attribute("vid"));
					}
					
					TiXmlElement* pchild=NULL;
					
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
				
				_basic_id_dic.insert(std::make_pair(key, _basic_dic_id_iter.size() - 1));	
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("udb", _basic_dic_name_iter.size() - 1));
				
			}else if (std::string (ent->d_name ) == "unisphere.xml")
			{
				uint8_t key;
				
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
	 		
				std::string name = path + "/unisphere.xml";
				
				TiXmlDocument doc ;
				
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
				
				TiXmlElement  *pcrElement=doc.RootElement();
				
				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
				
	 			while(pElementtmp)
				{
					
					if (strcmp(pElementtmp->Value(),"unisphere") == 0) 
					{
						key = atoi(pElementtmp->Attribute("vid"));
					}
					
					TiXmlElement* pchild=NULL;
					
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
				
				_basic_id_dic.insert(std::make_pair(key, _basic_dic_id_iter.size() - 1));	
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("unisphere", _basic_dic_name_iter.size() - 1));
			}else if (std::string (ent->d_name ) == "zte.xml")
			{
				uint8_t key; 
				
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
	 		
				std::string name = path + "/zte.xml";
				
				TiXmlDocument doc ;
				
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
				
				TiXmlElement  *pcrElement=doc.RootElement();
				
				TiXmlElement* pElementtmp=NULL;

				pElementtmp = pcrElement->FirstChildElement();;
				
	 			while(pElementtmp)
				{
					
					if (strcmp(pElementtmp->Value(),"zte") == 0) 
					{
						key = atoi(pElementtmp->Attribute("vid"));
					}
					
					TiXmlElement* pchild=NULL;
					
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
				
				_basic_id_dic.insert(std::make_pair(key, _basic_dic_id_iter.size() - 1));	
				
				_basic_dic_name_iter.push_back(pbasic_dic_name_container);
				
				_basic_name_dic.insert(std::make_pair("zte", _basic_dic_name_iter.size() - 1));
				
			}
		}
    }  
	return 0;
}

}