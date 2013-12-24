/*
 * Copyright(C) 2013,Company ghca. All rights reserved.
 * 
 * @file    configmanager.hpp
 * @brief   
 * @version 1.0
 * @author  kelinchuan
 * @data    
 */

#ifndef __CONFIG_MANAGER_HPP
#define __CONFIG_MANAGER_HPP

#include <map>
#include <vector>
#include <string>

#include <module/tinyxml/tinyxml.h>

namespace moudle{

	class CTreeNode;
	class CTreeNodeIterator;

	typedef std::map<std::string,std::string> Attribute;
	typedef std::map<std::string,CTreeNode*> NodeMap;

	class CTreeNode
	{
	public:
		CTreeNode():m_pParent(NULL),m_pNext(NULL)
		{}

		// 迭代器，轮训多同名结点的情况
		bool HasNext()
		{
			return (m_pNext == NULL);
		}

		CTreeNode* Next()
		{
			return m_pNext;
		}

		bool AddAttribute(const char *pname,const char *pvalue)
		{
			m_Attribute.insert(std::make_pair(pname,pvalue));
			return true;
		}

		std::string GetAttribute(const char *pname)
		{
			Attribute::iterator it = m_Attribute.find(pname);
			if(it != m_Attribute.end())
			{
				return it->second;
			}
			return "";
		}

		bool AddChild(const char *pname,CTreeNode *pNode)
		{
			if(m_ChildMap.find(pname) == m_ChildMap.end())
			{
				m_ChildMap.insert(std::make_pair(pname,pNode));
			}
			else
			{
				CTreeNode* pTmp = m_ChildMap.find(pname)->second;

				// 对于有多同名结点的情况
				while(pTmp->m_pNext)
				{
					pTmp = pTmp->m_pNext;
				}
				pTmp->m_pNext = pNode;
				pNode->m_pParent = pTmp->m_pParent;
			}
			return true;
		}

		CTreeNode *m_pParent;	// 指向父亲的指针				
		
		CTreeNode *m_pNext;		// 同名兄弟结点,如果有，在当前父结点下

		int m_ntype;			// 0该结点属于单名多结点，1，该结点可能存在多个
	
		std::string m_strname;  // 自身名称

		NodeMap m_ChildMap;		// 该结点的子结点表，如果

		Attribute m_Attribute;	// 该结点的属性表
	};

	class CTreeNodeIterator
	{

	public:
		CTreeNodeIterator():m_pNode(NULL)
		{}

		CTreeNodeIterator(CTreeNode* pNode)
		{
			m_pNode = pNode;
		}

		CTreeNodeIterator operator = (CTreeNodeIterator iter)
		{
			return iter;
		}

		CTreeNodeIterator& operator = (CTreeNode *pNode)
		{
			m_pNode = pNode;
			return *this;
		}

	public:
		
		bool IsEnd()
		{
			return m_pNode == NULL;
		}

		CTreeNodeIterator& Next()
		{
			m_pNode = m_pNode->Next();
			return (*this);
		}

		std::string GetAttribute(std::string attri)
		{
			if(!IsEnd())
			{
				return m_pNode->GetAttribute(attri.c_str());
			}
		}

		std::string GetTextValue(std::string attri)
		{
			if(!IsEnd())
			{
				return m_pNode->GetAttribute("TEXTVALUE");
			}
		}

	private:

		CTreeNode *m_pNode;
	};


	class CConfigManager
	{
	public:

		CConfigManager();

		~CConfigManager();

	public:

		/****
		根据配置文件路径加载内存
		参数一：配置文件名（xml必须以root为根结点)
		参数:配置文件的路径（全路径）
		***/
		int Init(std::string configName,const char *pPath);

		/****
		根据结点路径返回结点的属性值，注意（文本的值统一用 TEXTVALUE)
		参数一:配置文件名(你可以用xml的文件名，也可自定义，但是使用时必须一致
		参数二：结点的url
		<a>
			<b>
				<c></c>
			</b>
		</a>
		要查C结点的属性，url就可以为a.b.c形式
		参数三：属性名称
		***/
		std::string GetAttribute(std::string cfgname,std::string nodeUrl,std::string attrName);

		/**
		获取该结点文本值
		**/
		std::string GetTextValue(std::string cfgname,std::string nodeUrl);

		/*****
		如果同一父节点下有多同名结点，同名结点的第一个位置，然后通过迭代可以访问
		***/
		CTreeNodeIterator GetMulitNode(std::string cfgname,std::string nodeUrl);

	private:

		int  split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",");

		void freeNode(CTreeNode *pRoot);

		char* ShowNodeInfo(TiXmlElement *pCurrentNode,CTreeNode* current);

	private:

		NodeMap m_configMap;

	};
}

#endif	//__CONFIG_MANAGER_HPP
