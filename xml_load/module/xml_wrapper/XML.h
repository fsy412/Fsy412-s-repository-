#include<string>

#include "tinyxml.h"

using namespace std;

class CXML

{

public:

    CXML(void)

    {

    }

    ~CXML(void)

    {

    }

private:

    TiXmlDocument m_xml;

    TiXmlElement* pElement;

private:

    TiXmlElement* getFirstElement(string ElementMark,TiXmlElement* pcrElement);

public:

    //����xml�ַ���

    int ParseXmlStr(string xmlstr);

    //����xml�ļ�

    int ParseXmlFile(string xmlFile);

    //���ݱ�ǩȡֵ

    int getFirstElementValue(string ElementMark,string& value);

    //���ͬһ��ǩ�ļ�¼ȡֵ,�������ֵ��0�������޴˱�ǩ����ֵ��ȡ

    int getNextElementValue(string ElementMark,string& value);

    //ȡ������ֵ

    int getElementAttributeValue(string AttributeName,string& value);

    //��ȡ�����

    TiXmlElement* getRootElement();

    //���ص�ǰ��xml�ַ���

    string getXmlStr();

    //��ս���������

    void Clear();

    //����ӽڵ�

    TiXmlElement* addXmlRootElement(string ElementMark);//���һ�����ڵ�

    //����ӽڵ�

    TiXmlElement* addXmlChildElement(TiXmlElement* pElement,string ElementMark);

    //���ڵ����ֵ

    void addElementValue(TiXmlElement* pElement,string value);

    //������Լ�����ֵ

    void addXmlAttribute(TiXmlElement* pElement,string AttributeMark,string value);

    //�������

    void addXmlDeclaration(string vesion,string encoding,string standalone);

    //���ע��

    void addXmlComment(TiXmlElement* pElement,string Comment);

    //��xml���ݱ��浽�ļ�

    void saveFile(string FileName);

};