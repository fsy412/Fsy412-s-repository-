#include "XML.h"
#include <iostream>
int CXML::ParseXmlFile(string xmlFile)

{

    int result=0;

    try

    {
 
        if(m_xml.LoadFile(xmlFile.c_str()))
		{
            result=1;
 
		}
        else

            result=0;

    }

    catch(...)

    {

    }

    return result;

}

int CXML::ParseXmlStr(std::string xmlStr)

{

    int result=0;

    if(xmlStr=="")

        return 0;

    try

    {

        if(m_xml.Parse(xmlStr.c_str()))

            result=1;

        else

            result=0;

    }

    catch(...)

    {

    }

    return result;

}

TiXmlElement* CXML::getFirstElement(string ElementMark,TiXmlElement* pcrElement)

{

    TiXmlElement* pElementtmp=NULL;

    pElementtmp=pcrElement;

    while(pElementtmp)

    {

        if(strcmp(pElementtmp->Value(),ElementMark.c_str())==0)

        {
		 
            printf("%s\r\n",pElementtmp->Value());

            return pElementtmp;

        }

        else

        {

            TiXmlElement* nextElement=pElementtmp->FirstChildElement();

            while(nextElement)

            {

                printf("while nextElement %s\r\n",nextElement->Value());

                if(strcmp(nextElement->Value(),ElementMark.c_str())==0)

                {
			
                    return nextElement;

                }

                else

                {

                    TiXmlElement* reElement=NULL;

                    reElement=getFirstElement(ElementMark,nextElement);

                    if(reElement)

                    {

                        return reElement;

                    }

                }

                nextElement=nextElement->NextSiblingElement();

            }

        }

        pElementtmp=pElementtmp->NextSiblingElement();

    }

    return NULL;

}

//根据标签取值

int CXML::getFirstElementValue(string ElementMark,string& value)

{

    int result=0;

    if(ElementMark=="")

        return 0;

    try

    {

        TiXmlElement* pcrElement=NULL;

        pcrElement=m_xml.RootElement();

        pcrElement=this->getFirstElement(ElementMark,pcrElement);
				std::cout<<"attr "<<pcrElement->Attribute("name")<<std::endl;
				std::cout<<"attr "<<pcrElement->Attribute("brate")<<std::endl;

        if(pcrElement)

        {
			std::cout<<pcrElement->GetText();
 
            result=1;

        }

    }

    catch(...)

    {

    }

    return result;

}

int CXML::getNextElementValue(string ElementMark,string& value)

{

    value="";

    this->pElement=this->pElement->NextSiblingElement(ElementMark.c_str());

    if(this->pElement)

    {

        value=this->pElement->GetText();

        return 1;

    }

    return 0;

}

string CXML::getXmlStr()

{

    string result="";

    try

    {

        TiXmlPrinter printer;

        m_xml.Accept(&printer);

        result=printer.CStr();

    }

    catch(...)

    {

    }

    return result;

}

void CXML::Clear()

{

    m_xml.Clear();

}

//添加子节点

TiXmlElement* CXML::addXmlRootElement(string ElementMark)

{

    TiXmlElement* RootElement=new TiXmlElement(ElementMark.c_str());

    m_xml.LinkEndChild(RootElement);

    return RootElement;

}

TiXmlElement* CXML::addXmlChildElement(TiXmlElement* pElement,string ElementMark)

{

    if(pElement)

    {

        TiXmlElement* tempElement=new TiXmlElement(ElementMark.c_str());

        pElement->LinkEndChild(tempElement);

        return tempElement;

    }

    return 0;

}

void CXML::addElementValue(TiXmlElement *pElement, std::string value)

{

    if(pElement)

    {

        TiXmlText *pContent=new TiXmlText(value.c_str());

        pElement->LinkEndChild(pContent);

    }

}

//添加属性及属性值

void CXML::addXmlAttribute(TiXmlElement* pElement,string AttributeMark,string value)

{

    if(pElement)

    {

        pElement->SetAttribute(AttributeMark.c_str(),value.c_str());

    }

}

//添加声明

void CXML::addXmlDeclaration(string vesion,string encoding,string standalone)

{

    TiXmlDeclaration *pDeclaration=new TiXmlDeclaration(vesion.c_str(),encoding.c_str(),standalone.c_str());

    m_xml.LinkEndChild(pDeclaration);

}

//添加注释

void CXML::addXmlComment(TiXmlElement* pElement,string Comment)

{

    if(pElement)

    {

        TiXmlComment *pComment=new TiXmlComment(Comment.c_str());

        pElement->LinkEndChild(pComment);

    }

}

TiXmlElement* CXML::getRootElement()

{

    return m_xml.RootElement();

}

//取得属性值

int CXML::getElementAttributeValue(string AttributeName,string& value)

{

    if(this->pElement->Attribute(AttributeName.c_str()))

    {

        value=this->pElement->Attribute(AttributeName.c_str());

        return 1;

    }

    return 0;

}

void CXML::saveFile(string FileName)

{

    this->m_xml.SaveFile(FileName.c_str());

}