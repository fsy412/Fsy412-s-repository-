#include <dic.h>
#include <tinyxml.h>
   
 namespace ghca{ 



 
 
void dump_container(basic_dic_id_container *p)
{
	basic_dic_id_container::iterator it = p->begin();
	for(;it != p->end(); it++)
	{
		basic_dic_avp avp = it->second;
		 std::cout<<"avp"<<std::endl;
		 printf("id:%d\n", it->second.id);
		 printf("id:%d\n", it->second.type);
		 printf("id:%s\n", it->second.name.c_str());
		 // std::cout<<" id:"<<it->second.id<<std::endl;
		 // std::cout<<" type:"<<it->second.type<<std::endl;
		 // std::cout<<" name:"<<it->second.name<<std::endl;
	}	
}
void  dump_basic_dic_id_container()
{
	
}
int dictionary::scan(std::string path)
{
	DIR              *pDir ;  
    struct dirent    *ent  ;  
    int               i=0  ;  
    char              childpath[512];  
	std::cout<<"open path "<<path<<std::endl;
    pDir=opendir(path.c_str());  
    memset(childpath,0,sizeof(childpath));  
	int pos = 0;
	uint8_t pos_basic_id_dic = 0;
  
    while((ent=readdir(pDir))!=NULL)  
    {  
        if(ent->d_type & DT_DIR)  
        {  
			continue;  
    	}  
		else
		{
			std::cout<<ent->d_name<<std::endl;
				
			if (std::string (ent->d_name ) == "rfc.xml")
			{
				basic_dic_id_container *pbasic_dic_id_container = NULL;
				
				basic_dic_name_container *pbasic_dic_name_container = NULL;
				
				std::string name = path + "/rfc.xml";
				
				TiXmlDocument doc ;
				
				std::cout<<"Load file"<<name<<std::endl;
				
				if (!doc.LoadFile(name.c_str()))
				{
					std::cout << "load xml failed!" << std::endl;
					return 1;
				}
							
				pbasic_dic_id_container = new basic_dic_id_container();
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
								printf("address pbasic_dic_id_container %x:\n", pbasic_dic_id_container);
				printf("address :%x\n", _basic_dic_id_iter[_basic_id_dic.find(_basic_id_dic.size() - 1)->second]);
				//printf("_basic_id_dic: %x\n", _basic_id_dic.find(key)->second);
				
			
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
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
					
						printf("key:%d\n",key);
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
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
				
						printf("key:%d\n",key);
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
				
				printf("address pbasic_dic_id_container %x:\n", pbasic_dic_id_container);
				printf("address :%x\n", _basic_dic_id_iter[_basic_id_dic.find(key)->second]);
				printf("_basic_id_dic: %x\n", _basic_id_dic.find(key)->second);
			
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
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
				
						printf("key:%d\n",key);
					
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
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
				printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
	 			while(pElementtmp)
				{
					
					if (strcmp(pElementtmp->Value(),"udb") == 0) 
					{
					
						key = atoi(pElementtmp->Attribute("vid"));
				
						printf("key:%d\n",key);
					
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
				
				std::cout<<"dump _basic_id_dic"<<std::endl;
				for (basic_id_dic::iterator it=_basic_id_dic.begin(); it!=_basic_id_dic.end(); ++it)
					{
						printf("%d => %d\n", it->first,   it->second);
						printf("_basic_dic_id_iter address %x:\n ", _basic_dic_id_iter[it->second]);
					//std::cout << "_basic_id_dic  :"<<it->first << " => " << it->second << '\n';
					
					}
					
				std::cout<<"dump _basic_name_dic"<<std::endl;
				for (basic_name_dic::iterator it=_basic_name_dic.begin(); it!=_basic_name_dic.end(); ++it)
					{
						printf("%s => %d\n", it->first.c_str(),   it->second);
						printf("_basic_dic_id_iter address %x:\n ", _basic_dic_id_iter[it->second]);
					//std::cout << "_basic_id_dic  :"<<it->first << " => " << it->second << '\n';
					}
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
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
				
						printf("key:%d\n",key);
					
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
					printf("basic_dic_id_container address %x\n", pbasic_dic_id_container);
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
				
						printf("key:%d\n",key);
					
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
				
				std::cout<<"dump _basic_id_dic"<<std::endl;
				for (basic_id_dic::iterator it=_basic_id_dic.begin(); it!=_basic_id_dic.end(); ++it)
					{
						printf("%d => %d\n", it->first,   it->second);
					//std::cout << "_basic_id_dic  :"<<it->first << " => " << it->second << '\n';
					}
					
				std::cout<<"dump _basic_name_dic"<<std::endl;
				for (basic_name_dic::iterator it=_basic_name_dic.begin(); it!=_basic_name_dic.end(); ++it)
					{
						printf("%s => %d\n", it->first.c_str(),   it->second);
					//std::cout << "_basic_id_dic  :"<<it->first << " => " << it->second << '\n';
					}
			}
		}
    }  
	return 0;
}

}