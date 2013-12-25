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

    //解析xml字符串

    int ParseXmlStr(string xmlstr);

    //解析xml文件

    int ParseXmlFile(string xmlFile);

    //根据标签取值

    int getFirstElementValue(string ElementMark,string& value);

    //针对同一标签的记录取值,如果返回值是0表明再无此标签内容值可取

    int getNextElementValue(string ElementMark,string& value);

    //取得属性值

    int getElementAttributeValue(string AttributeName,string& value);

    //获取根结点

    TiXmlElement* getRootElement();

    //返回当前的xml字符串

    string getXmlStr();

    //清空解析的内容

    void Clear();

    //添加子节点

    TiXmlElement* addXmlRootElement(string ElementMark);//添加一个根节点

    //添加子节点

    TiXmlElement* addXmlChildElement(TiXmlElement* pElement,string ElementMark);

    //给节点添加值

    void addElementValue(TiXmlElement* pElement,string value);

    //添加属性及属性值

    void addXmlAttribute(TiXmlElement* pElement,string AttributeMark,string value);

    //添加声明

    void addXmlDeclaration(string vesion,string encoding,string standalone);

    //添加注释

    void addXmlComment(TiXmlElement* pElement,string Comment);

    //将xml内容保存到文件

    void saveFile(string FileName);

};