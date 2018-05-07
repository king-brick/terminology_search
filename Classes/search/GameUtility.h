#ifndef __GAME_UTILITY__
#define __GAME_UTILITY__

#include<sstream>
#include<stdio.h> //定义输入／输出函数
#include<stdlib.h> //定义杂项函数及内存分配函数 
#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include "cocos2d.h"
#include "Common.pb.h"

USING_NS_CC;
using namespace std;
using namespace google::protobuf;

#define GetBitFlag(i, index) ((i >> (index-1)) & 1) // 取i的第index位(index从1开始)

//字符串拆分
extern void split(vector<string>& vecOutput, const string& strInput);
extern void split(vector<string>& vecOutput, const string& strInput, const char* token);
//字符串转换到其他类型
extern int Convert_Int(const string& strInput);
extern float Convert_Double(const string& strInput);
//字符串转换到int数组
extern void Convert_Value(vector<int>& vecField, const string& strInput);
extern void Convert_Value(vector<vector<int>>& vecField, const string& strInput);

extern int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);
extern string GBK2Utf8(const char *inbuf);
extern string Utf8ToGBK(const char *inbuf);

template <class T>
string ToString(T t)
{
	static stringstream ss;
	ss.clear();
	ss.sync();
	ss.str("");
	ss<<t;
	string str;
	ss>>str;
	return str;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
template <>
inline string ToString<int>(int t)
{
	return tdgame::i2a(t);
}
#endif

template <class T>
T FromString(const string& str)
{
	static stringstream ss;
	ss.clear();
	ss.sync();
	ss.str("");
	ss<<str;
	T t;
	ss>>t;
	return t;
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
template<>
inline int FromString(const string& str)
{
	return std::atoi(str.c_str());
}

template<>
inline float FromString(const string& str)
{
	return std::atof(str.c_str());
}
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
template<>
inline int FromString(const string& str)
{
	return std::atoi(str.c_str());
}

template<>
inline float FromString(const string& str)
{
	return std::atof(str.c_str());
}
#endif


// 删除map里的指针并清空map
template <class T>
void deleteTable(map<int, T*> &tbl)
{
	typename map<int, T*>::iterator iter = tbl.begin();
	for (; iter != tbl.end(); ++iter)
	{
		if (NULL != iter->second)
		{
			delete iter->second;
		}
	}

	tbl.clear();
}

extern unsigned char* getFileDataByCocos2d(const char* pszFileName, unsigned long& uSize);

template <class TT>
void Table_Init(string _path, map<int, TT*> &_table_name)
{
	try {
		unsigned long uSize = 0;
		char* pData = reinterpret_cast<char*>(getFileDataByCocos2d(_path.c_str(), uSize));
		if(NULL == pData || 0 == uSize)
			return;

		string strData(pData, uSize);
		stringstream ss;
		ss << strData;
		string strLine;
		vector<string> vecField;
		int state = 1;
        
		while(!ss.eof() && getline(ss, strLine))
		{
			if (0 == strLine.size() || strLine[0]=='#')
			{
				continue;
			}
			//加载 数据类型
			else if(state==1)
			{
				state=2;
			}
			else if(state==2)  //加载 列名
			{
				state=3;
			}
			//逐行加载
			else if(state==3)
			{
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
				int len = strLine.size();
				if('\r' == strLine[len-1])
				{// delete the last \r
					strLine = strLine.substr(0, len-1);
				}
#endif
				vecField.clear();
				split(vecField, strLine);

				if(0 < vecField.size())
				{
					int nID = Convert_Int(vecField[0]);
					TT* pTT = new TT(vecField);
					_table_name[nID] = pTT;
				}
			}
		}

		ss.str("");
		CC_SAFE_DELETE_ARRAY(pData);
	}
	catch(...)
	{
		// throw exception
	}
}

// 得到当前客户端时间，返回秒数
int getCurrentClientTime();
// 得到详细的时间，取得：年月日天时分秒
void getDetailTime( int nSecondCoun, int *year=NULL, int *month=NULL, int *day=NULL, int *hour=NULL, int *minute=NULL, int *second=NULL );

// 判断两个时间是否同一天
bool isInSameDay(int nTime1, int nTime2);

extern string md5Pwd(const string& pwd);

void ReplaceScene(CCScene* pScene,float dt=0.5f);

// 在字符串中每个字符后面添加一个字符（汉字为1个字符）
extern void AddCharAfterOneChar(const string &src, char cAdd, string &result);

bool IsEmptyString(const char* info);
bool IsHaveIllegalChar(const char* info);

// 剪裁图片
CCSprite* maskedSpriteWithSprite(CCSprite* textureSprite, CCSprite* maskSprite);
// 截图
bool screenShot(CCNode* pNode, const string& sFileName);
// 截图返回CCRenderTexture
CCSprite* getRenderTexture(CCNode* node);
// 字符替换
void string_replace(string& strBig, const string & strsrc, const string &strdst);
// 把node转换为图片
CCSprite* createSpriteByNode(CCNode* pNode);

class BitFlag
{
public:
	BitFlag(int nFlagCount);
	BitFlag(int nFlagCount, int InitValue);

public:
	int GetFlag(int nIndex);
	void SetFlag(int nIndex);
	void CleanFlag(int nIndex);
	void Init(const int* pData);
	void ReSetAllFlag();

private:
	int m_nCount;
	vector<int> m_Flag;
};


class LambdaCallFunc : public CCCallFunc
{
public:
	static LambdaCallFunc* create(function<void(void)> callback)
	{
		LambdaCallFunc *pCallFunc = new LambdaCallFunc();
		pCallFunc->autorelease();
		pCallFunc->_Callback = callback;
		return pCallFunc;
	}
	virtual void execute(void)
	{
		if(_Callback)
		{
			_Callback();
		}
	}
private:
	function<void(void)> _Callback;
};

class  GameException
{
public:
	GameException(const char* str):exStr(str)  {}
	virtual ~GameException(){};
	virtual const char* what() const {return exStr.c_str();}
private:
	string exStr;
};

extern const int calculateUtf8StringLength(const char* utf8Str, int* pMultiBytesCharCount);

extern uint64 b24toi(const char *ptr, size_t count);

extern char *itob24(uint64 str, char *buf, size_t count);

string utf8_substr(const string& str,unsigned int pos,int len);
string utf8_substr_by_charlength(const string& str,int len);

#endif	//!__GAME_UTILITY__
