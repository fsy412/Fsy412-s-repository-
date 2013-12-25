#include "configmanager.hpp"

namespace moudle{

	CConfigManager::CConfigManager()
	{}

	CConfigManager::~CConfigManager()
	{
		NodeMap::iterator i = m_configMap.begin();
		while(i!=m_configMap.end())
		{
			freeNode(i->second);
			i++;
		}
	}

	void CConfigManager::freeNode(CTreeNode *pRoot)
	{
		NodeMap::iterator i = pRoot->m_ChildMap.begin();
		while(i != pRoot->m_ChildMap.end())
		{
			freeNode(i->second);
			i++;
		}
		//子结点释放完则释放本结点
		delete pRoot;
	}

	int CConfigManager::split(const std::string& str, std::vector<std::string>& ret_, std::string sep)
	{
		if(str.empty())
		{
			return 0;
		}
		std::string tmp;
		std::string::size_type pos_begin = str.find_first_not_of(sep);
		std::string::size_type comma_pos = 0;

		while (pos_begin != std::string::npos)
		{
			comma_pos = str.find(sep, pos_begin);
			if (comma_pos != std::string::npos)
			{
				tmp = str.substr(pos_begin, comma_pos - pos_begin);
				pos_begin = comma_pos + sep.length();
			}
			else
			{
				tmp = str.substr(pos_begin);
				pos_begin = comma_pos;
			}

			if (!tmp.empty())
			{
				ret_.push_back(tmp);
				tmp.clear();
			}
		}
		return 0;
	}

	CTreeNodeIterator CConfigManager::GetMulitNode(std::string cfgname,std::string nodeUrl)
	{
		int index = nodeUrl.find_first_of('.');
		std::vector<std::string> ret_;
		this->split(nodeUrl,ret_,".");
		std::vector<std::string>::iterator it=ret_.begin();
		CTreeNode* pRootNode=this->m_configMap[cfgname];
		if(pRootNode == NULL)
		{
			return CTreeNodeIterator(NULL);
		}
		CTreeNode* pTmp=pRootNode;
		int find=0;
		while(it != ret_.end()&&(*it) == "root")//掠过空或者root开头的
		{
			it++;
		}
		while(it!=ret_.end())
		{
			std::string nodeFind = (*it);
			if(nodeFind=="")//掠过root结点
			{  
				it++;
				continue;
			}
			NodeMap::iterator itNode=pTmp->m_ChildMap.find(nodeFind);

			if(itNode!=pTmp->m_ChildMap.end())
			{
				pTmp=(itNode)->second;//同时修改ptmp指针
				it++;
			}
			else
			{
				find=-1;
				break;
			}
		}
		if(find==-1)
		{

			return CTreeNodeIterator(NULL);
		}
		CTreeNodeIterator ss=CTreeNodeIterator(pTmp);
		CTreeNodeIterator* sss=&ss;
		return ss;
	}

	std::string CConfigManager::GetAttribute(std::string cfgname,std::string nodeUrl,std::string attrName)
	{
		int index=nodeUrl.find_first_of('.');
		std::vector<std::string> ret_;
		this->split(nodeUrl,ret_,".");
		std::vector<std::string>::iterator it=ret_.begin();
		CTreeNode* pRootNode = this->m_configMap[cfgname];
		if(pRootNode == NULL)
		{
			return "";
		}
		CTreeNode* pTmp=pRootNode;
		int find=0;
		while(it!=ret_.end()&&(*it)=="root")//掠过空或者root开头的
		{
			it++;
		}
		while(it!=ret_.end())
		{
			std::string nodeFind=(*it);
			if(nodeFind=="")//掠过root结点
			{  
				it++;
				continue;
			}
			NodeMap::iterator itNode=pTmp->m_ChildMap.find(nodeFind);
			if(itNode!=pTmp->m_ChildMap.end())
			{
				pTmp=(itNode)->second;//同时修改ptmp指针
				it++;
			}
			else
			{
				find=-1;
				break;
			}
		}
		if(find==-1)
		{
			return "";
		}
		std::map<std::string,std::string>::iterator attrIter=pTmp->m_Attribute.find(attrName);
		if(attrIter!=pTmp->m_Attribute.end())
		{
			return attrIter->second;
		}
		else
		{
			return "";
		}
	}

	std::string CConfigManager::GetTextValue(std::string cfgname,std::string nodeUrl)
	{
		return GetAttribute(cfgname, nodeUrl,"TEXTVALUE");
	}

	int CConfigManager::Init(std::string configName,const char* pPath)
	{
		if(pPath == NULL)
		{
			return -1;
		}

		TiXmlDocument doc;
		std::string configxml;
		configxml += pPath;

		if(!doc.LoadFile(configxml.c_str()))
		{
			return -1;
		}

		TiXmlHandle docHandle( &doc );
		TiXmlElement* eleMent = docHandle.FirstChild("root").ToElement();

		CTreeNode* pRootNode = new CTreeNode();
		pRootNode->m_strname = "root";
		pRootNode->m_pNext = NULL;
		pRootNode->m_pParent = NULL;

		ShowNodeInfo(eleMent,pRootNode);

		m_configMap.insert(std::make_pair(configName,pRootNode));

		return 0;
	}

	//为NULL时表示一次递归完成	
	char *CConfigManager::ShowNodeInfo(TiXmlElement *pCurrentNode,CTreeNode* current)
	{
		//为NULL时表示一次递归完成
		if(NULL == pCurrentNode )
		{
			return NULL;
		}
		else if ( pCurrentNode->NoChildren()&&pCurrentNode->Type()==TiXmlText::TINYXML_ELEMENT)//没有子结点则是最底层的结点
		{
			TiXmlAttribute* attr =pCurrentNode->FirstAttribute();
			while(attr)
			{
				current->AddAttribute((char*)attr->Name(),(char*)attr->Value());
				attr=attr->Next();

			}
			const char* testValue=pCurrentNode->GetText();
			if(testValue!=NULL&&strcmp(testValue,"")==0)
			{
				current->AddAttribute("TEXTVALUE",(char*)testValue);
			}
			ShowNodeInfo(NULL,NULL);
		}
		else if( ! pCurrentNode->NoChildren() )//有子结点则每个子结点都递归处理
		{   

			TiXmlAttribute* attr = pCurrentNode->FirstAttribute();
			while(attr)
			{
				current->AddAttribute((char*)attr->Name(),(char*)attr->Value());
				attr=attr->Next();
			}
			const char* testValue = pCurrentNode->GetText();
			if(testValue != NULL && strcmp(testValue,"") != 0)
			{
				current->AddAttribute("TEXTVALUE",(char*)testValue);
			}
			TiXmlElement * pChilds = pCurrentNode->FirstChildElement();//获取第一个子结点
			if(NULL != pChilds)
			{
				CTreeNode* tmp=new CTreeNode();
				tmp->m_pNext=NULL;
				tmp->m_pParent=current;//指向父结点
				tmp->m_strname=(char*)pChilds->Value();
				current->AddChild((char*)pChilds->Value(),tmp);
				ShowNodeInfo(pChilds,tmp);//递归子结点	
			}
			else
			{
				ShowNodeInfo(pChilds,NULL);

			}
			if(NULL != pChilds)
			{
				pChilds = pChilds->NextSiblingElement();// 获取刚显示的结点的同级下一个结点
				while ( NULL != pChilds&&pChilds->Type()==TiXmlText::TINYXML_ELEMENT)//递归处理此结点下的所有结点
				{
					CTreeNode* tmp = new CTreeNode();
					tmp->m_pParent = current;  // 指向父结点
					tmp->m_strname = (char*)pChilds->Value();
					tmp->m_pNext = NULL;
					current->AddChild((char*)pChilds->Value(),tmp);
					ShowNodeInfo(pChilds,tmp);
					pChilds = pChilds->NextSiblingElement();
				}
			}
			ShowNodeInfo(NULL,NULL);// 此结点所有子结点都处理完毕
		}
		return NULL;
	}

}
