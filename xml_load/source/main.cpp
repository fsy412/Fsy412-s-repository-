 
 
 
#include <iostream>
 

 
#include <tinyxml.h>
#include <XML.h>
#include <string>
 

int main()
{
	TiXmlDocument doc("app.xml");
	if (!doc.LoadFile())
	{
		std::cout<<"failed to load file"<<std::endl;

	}

	TiXmlElement   *pcrElement=doc.RootElement();

	TiXmlElement* pElementtmp=NULL;

	pElementtmp=	pcrElement->FirstChildElement();;

	std::cout<<"traval root "<<std::endl;
	while(pElementtmp)
	{


		std::cout<<"print node "<<std::endl;

		std::cout << pElementtmp->Value()<<std::endl;

		TiXmlElement* pchild=NULL;

		pchild = pElementtmp->FirstChildElement();
		while(pchild)
		{
			std::cout << pchild->Value()<<std::endl;
			if (strcmp(pchild->Value(),"port" ) ==  0)
			{
				std::cout<<"attr "<<pchild->Attribute("name")<<std::endl;
			}
			

			pchild = pchild ->NextSiblingElement();
		}


		pElementtmp=pElementtmp->NextSiblingElement();

	}

 

	//doc.Print();

	CXML xml;
	if (xml.ParseXmlFile("/home/share/xml_load/bin/app.xml"))
	{
		std::cout<<"xml load OK"<<std::endl;
	}
	std::string value;
	int ret = xml.getFirstElementValue("port", value);

	if (ret )
	{
		std::cout<<value<<std::endl;
	}
	return 0;
}
