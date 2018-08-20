#include "SearchInfo.h"
#include "CSV.hpp"
#include <iostream>
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <iconv.h>
#endif

static const string c_str_vocabulary_file = "vocabulary.csv";
static const string c_str_agency_file = "agency.csv";
static const string c_str_group_file = "group.csv";

using namespace cocos2d;
using namespace MySearch;

std::string getFileData(const char * pFileName)
{
	std::string filePath = FileUtils::getInstance()->fullPathForFilename(pFileName);

	FileUtils::getInstance()->setPopupNotify(false);
	ssize_t fileSize = 0;
	std::string data = FileUtils::getInstance()->getStringFromFile(filePath.c_str());
	FileUtils::getInstance()->setPopupNotify(true);

	return data;
}

//字符串拆分  
std::vector< std::string> splitString(std::string str, std::string pattern)
{
	//双引号内的内容不做拆分
	bool bFindQuotation = false;
	unsigned int start = 0;
	std::string::size_type pos;
	std::vector< std::string> result;
	str += pattern;
	unsigned int size = str.size();
	for (unsigned int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);

			//双引号内的内容不做拆分
			int nQuotation = count(s.begin(), s.end(), '"');
			if (0 < nQuotation && nQuotation % 2 != 0)
			{
				if (bFindQuotation)
				{
					bFindQuotation = false;
					std::string s1 = str.substr(start, pos - start);
					
					result.push_back(s1);
				}
				else
				{
					bFindQuotation = true;
					start = i;
				}
			}
			else if(!bFindQuotation)
			{
				// 移除首尾的"
				if (s.find_first_of("\"") == 0)
				{
					s.erase(0, 1);
					s.erase(s.find_last_of("\""), 1);
				}
				result.push_back(s);
			}
			
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

MySearch::DictionaryManager MySearch::g_Dictionary;

DictionaryManager::DictionaryManager()
{

}

DictionaryManager::~DictionaryManager()
{

}

void DictionaryManager::LoadData()
{
	cout << "begin load data......" << endl;
	LoadVocabulary();
	LoadAgency();
	LoadGroup();
	cout << "end load data......" << endl;
}

const SSimpleIndex* DictionaryManager::FindAll(string input)
{
	map<string, SSimpleIndex>::iterator result = AllIndex.find(input);
	if (AllIndex.end() != result)
	{
		return &result->second;
	}
	return nullptr;
}

vector<SSimpleIndex> DictionaryManager::FindLikeAll(string input)
{
	vector<SSimpleIndex> result;
	if (!input.empty())
	{
		if (input == SaveLastIndex)
		{// 相同的搜索
			return CacheSearchResult;
		}

		size_t SearchSize = input.size();
		if (0 < CacheSearchResult.size())
		{
			if (SaveLastIndex.size() < SearchSize)
			{
				string tmp = input.substr(0, SaveLastIndex.size());
				if (tmp == SaveLastIndex)
				{
					// 从上次搜索结果中查找
					for (auto e : CacheSearchResult)
					{
						string key = e.Index;
						std::transform(key.begin(), key.end(), key.begin(), ::tolower);
						if (key.size() < SearchSize)
							continue;

						string shortKey = key.substr(0, SearchSize);
						if (shortKey == input)
						{
							result.push_back(e);
						}
					}

					SaveLastIndex = input;
					CacheSearchResult = result;
					return result;
				}
			}
		}

		for (auto e : AllIndex)
		{
			string key = e.first;
			if (key.size() < SearchSize)
				continue;

			string shortKey = key.substr(0, SearchSize);
			if (shortKey == input)
			{
				result.push_back(e.second);
			}
		}

		SaveLastIndex = input;
		CacheSearchResult = result;
	}
	return result;
}

const SSimpleIndex* DictionaryManager::GetSearchIndex(string input)
{
	if (0 >= CacheSearchResult.size())
		return nullptr;
	for (int i = 0; i < CacheSearchResult.size(); ++i)
	{
		auto e = CacheSearchResult[i];
		if (e.Index == input)
		{
			return &CacheSearchResult[i];
		}
	}

	return nullptr;
}

/*
vector<SVocabulary> DictionaryManager::SearchVocabulary(string input)
{
	vector<SVocabulary> result;

	// 精确查找
	vector<int> r = FindVocabularyByEnglish(input);
	if (r.empty())
	{
		r = FindVocabularyByChinese(input);
	}
	for (auto i : r)
	{
		result.push_back(GetVocabularyInfo(i));
	}
	// 已找到
	if (0 < result.size())
	{
		return result;
	}

	// 认可词汇模糊搜索
	r = FindLikeVocabularyByEnglish(input);
	for (auto i : r)
	{
		result.push_back(GetVocabularyInfo(i));
	}

	if (r.empty())
	{
		r = FindLikeVocabularyByChinese(input);
		for (auto i : r)
		{
			result.push_back(GetVocabularyInfo(i));
		}
	}

	return result;
}

vector<SAgency> DictionaryManager::SearchAgency(string input)
{
	vector<SAgency> result;

	// 精确查找
	vector<int> r = FindAgencyByAbbrName(input);
	if (r.empty())
	{
		r = FindAgencyByChineseName(input);
	}
	for (auto i : r)
	{
		result.push_back(GetAgencyInfo(i));
	}

	// 已找到
	if (0 < result.size())
	{
		return result;
	}


	// 模糊查找
	r = FindAgencyByAbbrName(input);
	for (auto i : r)
	{
		result.push_back(GetAgencyInfo(i));
	}

	if (r.empty())
	{
		r = FindAgencyByChineseName(input);
		for (auto i : r)
		{
			result.push_back(GetAgencyInfo(i));
		}
	}
	return result;
}

vector<SCountryGroup> DictionaryManager::SearchGroup(string input)
{
	vector<SCountryGroup> result;
	// 精确查找
	vector<int> r = FindGroupByAbbrName(input);
	if (r.empty())
	{
		r = FindGroupByChinese(input);
	}
	for (auto i : r)
	{
		result.push_back(GetGroupInfo(i));
	}

	// 已找到
	if (0 < result.size())
	{
		return result;
	}


	// 模糊查找
	r = FindLikeGroupByAbbrName(input);
	for (auto i : r)
	{
		result.push_back(GetGroupInfo(i));
	}

	if (r.empty())
	{
		r = FindLikeGroupByChineseName(input);
		for (auto i : r)
		{
			result.push_back(GetGroupInfo(i));
		}
	}

	return result;
}
*/

void DictionaryManager::LoadVocabulary()
{
	cout << "load " << c_str_vocabulary_file.c_str() << endl;

	auto data = getFileData(c_str_vocabulary_file.c_str());
	// 读行
	auto lines = splitString(data, "\r\n");

	// 拆分行
	for (int i = 1; i < lines.size(); ++i)
	{
		auto fields = splitString(lines[i], ",");
		if (fields.size() < 8)
			continue;

		SVocabulary s;
		s.Base.id = atoi(fields[0].c_str());
		s.Base.English = fields[1];
		s.Base.Chinese = fields[2];
		s.Paraphrase = fields[3];
		s.EnglishSource = fields[4];
		s.EnglishNumber = fields[5];
		s.ChineseSource = fields[6];
		s.ChineseNumber = fields[7];

		AllVocabulary.insert(make_pair(s.Base.id, s));
		Add2AllIndex(s.Base, EType::Vocabulary);
	}
    /*
	std::string fullpath = FileUtils::getInstance()->fullPathForFilename(c_str_vocabulary_file);
    
	jay::util::CSVread csv_read(fullpath,
		jay::util::CSVread::strict_mode
		//| jay::util::CSVread::text_mode
		| jay::util::CSVread::process_empty_records
		//| jay::util::CSVread::skip_utf8_bom_check
	);
	if (csv_read.error)
		return;

	while (csv_read.ReadRecord())
	{
		SVocabulary s;
		s.Base.id = atoi(csv_read.fields[0].c_str());
		s.Base.English = csv_read.fields[1];
        
        //std::string cn = csv_read.fields[2];
        //int gblen = 1024;
        //char gbcn[1024] = {0};
        //convertutf8togbk((char*)cn.c_str(), (int)cn.length(), &gbcn[0], gblen);
        //s.Base.Chinese = gbcn;
        
        s.Base.Chinese = csv_read.fields[2];
		s.Paraphrase = csv_read.fields[3];
		s.EnglishSource = csv_read.fields[4];
		s.EnglishNumber = csv_read.fields[5];
		s.ChineseSource = csv_read.fields[6];
		s.ChineseNumber = csv_read.fields[7];

		AllVocabulary.insert(make_pair(s.Base.id, s));
		Add2AllIndex(s.Base, EType::Vocabulary);

// 		if (!s.Base.English.empty())
// 		{
// 			if (VocabularyEnglishIdx.end() != VocabularyEnglishIdx.find(s.Base.English))
// 			{
// 				VocabularyEnglishIdx[s.Base.English].push_back(s.Base.id);
// 			}
// 			else
// 			{
// 				vector<int> ids;
// 				ids.push_back(s.Base.id);
// 				VocabularyEnglishIdx.insert(make_pair(s.Base.English, ids));
// 			}
// 		}
// 
// 		if (!s.Base.Chinese.empty())
// 		{
// 			if (VocabularyChineseIdx.end() != VocabularyChineseIdx.find(s.Base.Chinese))
// 			{
// 				VocabularyChineseIdx[s.Base.Chinese].push_back(s.Base.id);
// 			}
// 			else
// 			{
// 				vector<int> ids;
// 				ids.push_back(s.Base.id);
// 				VocabularyChineseIdx.insert(make_pair(s.Base.Chinese, ids));
// 			}
// 		}
	}
	*/
	// 排序
}

void DictionaryManager::LoadAgency()
{
	cout << "load " << c_str_agency_file.c_str() << endl;

	auto data = getFileData(c_str_agency_file.c_str());
	// 读行
	auto lines = splitString(data, "\r\n");

	// 拆分行
	for (int i = 1; i < lines.size(); ++i)
	{
		auto fields = splitString(lines[i], ",");
		if (fields.size() < 5)
			continue;

		SAgency s;
		s.Base.id = atoi(fields[0].c_str());
		s.Base.Chinese = fields[2];
		s.Base.English = fields[4];
		s.Noun = fields[1];
		s.EnglishFullName = fields[3];
		s.ChineseFullName = fields[5];

		AllAgency.insert(make_pair(s.Base.id, s));
		Add2AllIndex(s.Base, EType::Agency);
	}


	/*
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(c_str_agency_file);
    
	jay::util::CSVread csv_read(fullpath,
		jay::util::CSVread::strict_mode
		//| jay::util::CSVread::text_mode
		| jay::util::CSVread::process_empty_records
		//| jay::util::CSVread::skip_utf8_bom_check
	);
	if (csv_read.error)
		return;

	while (csv_read.ReadRecord())
	{
		SAgency s;
		s.Base.id = atoi(csv_read.fields[0].c_str());
		s.Base.Chinese = csv_read.fields[1];
		s.Base.English = csv_read.fields[2];
		s.EnglishFullName = csv_read.fields[3];

		AllAgency.insert(make_pair(s.Base.id, s));
		Add2AllIndex(s.Base, EType::Agency);

// 		if (!s.Base.Chinese.empty())
// 		{
// 			if (AgencyChineseIdx.end() != AgencyChineseIdx.find(s.Base.Chinese))
// 			{
// 				AgencyChineseIdx[s.Base.Chinese].push_back(s.Base.id);
// 			}
// 			else
// 			{
// 				vector<int> ids;
// 				ids.push_back(s.Base.id);
// 				AgencyChineseIdx.insert(make_pair(s.Base.Chinese, ids));
// 			}
// 		}
// 
// 		if (!s.Base.English.empty())
// 		{
// 			if (AgencyAbbrIdx.end() != AgencyAbbrIdx.find(s.Base.English))
// 			{
// 				AgencyAbbrIdx[s.Base.English].push_back(s.Base.id);
// 			}
// 			else
// 			{
// 				vector<int> ids;
// 				ids.push_back(s.Base.id);
// 				AgencyAbbrIdx.insert(make_pair(s.Base.English, ids));
// 			}
// 		}
	}
	*/
	// 排序
}

void DictionaryManager::LoadGroup()
{
	cout << "load " << c_str_group_file.c_str() << endl;

	auto data = getFileData(c_str_group_file.c_str());
	// 读行
	auto lines = splitString(data, "\r\n");

	// 拆分行
	for (int i = 1; i < lines.size(); ++i)
	{
		auto fields = splitString(lines[i], ",");
		if (fields.size() < 4)
			continue;

		SCountryGroup s;
		s.Base.id = atoi(fields[0].c_str());
		s.Base.Chinese = fields[1];
		s.Base.English = fields[2];
		s.EnglishFullName = fields[3];
		s.ChineseFullName = fields[4];

		AllGroup.insert(make_pair(s.Base.id, s));
		Add2AllIndex(s.Base, EType::Group);
	}
	/*
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(c_str_group_file);
    
	jay::util::CSVread csv_read(fullpath,
		jay::util::CSVread::strict_mode
		//| jay::util::CSVread::text_mode
		| jay::util::CSVread::process_empty_records
		//| jay::util::CSVread::skip_utf8_bom_check
	);
	if (csv_read.error)
		return;

	while (csv_read.ReadRecord())
	{
		SCountryGroup s;
		s.Base.id = atoi(csv_read.fields[0].c_str());
		s.Base.Chinese = csv_read.fields[1];
		s.Base.English = csv_read.fields[2];
		s.EnglishFullName = csv_read.fields[3];

		AllGroup.insert(make_pair(s.Base.id, s));
		Add2AllIndex(s.Base, EType::Group);

// 		if (!s.Base.Chinese.empty())
// 		{
// 			if (GroupChineseIdx.end() != GroupChineseIdx.find(s.Base.Chinese))
// 			{
// 				GroupChineseIdx[s.Base.Chinese].push_back(s.Base.id);
// 			}
// 			else
// 			{
// 				vector<int> ids;
// 				ids.push_back(s.Base.id);
// 				GroupChineseIdx.insert(make_pair(s.Base.Chinese, ids));
// 			}
// 		}
// 
// 		if (!s.Base.English.empty())
// 		{
// 			if (GroupAbbrIdx.end() != GroupAbbrIdx.find(s.Base.English))
// 			{
// 				GroupAbbrIdx[s.Base.English].push_back(s.Base.id);
// 			}
// 			else
// 			{
// 				vector<int> ids;
// 				ids.push_back(s.Base.id);
// 				GroupAbbrIdx.insert(make_pair(s.Base.English, ids));
// 			}
// 		}
	}
	*/
	// 排序
}

void DictionaryManager::Add2AllIndex(const RecordBase& record, EType type)
{
	SIndexBase idx;
	idx.Type = type;
	idx.Id = record.id;
	// 英文名
	if (!record.English.empty())
	{
		string index = record.English;
		transform(index.begin(), index.end(), index.begin(), ::tolower);
		map<string, SSimpleIndex>::iterator result = AllIndex.find(index);
		if (AllIndex.end() == result)
		{
			SSimpleIndex s;
			s.Index = record.English;
			s.bEnglish = true;
			s.Base = record;
			s.ids.push_back(idx);
			AllIndex.insert(make_pair(index, s));
		}
		else
		{
			AllIndex[index].ids.push_back(idx);
		}
	}
	// 中文名
	if (!record.Chinese.empty())
	{
		map<string, SSimpleIndex>::iterator result = AllIndex.find(record.Chinese);
		if (AllIndex.end() == result)
		{
			SSimpleIndex s;
			s.Index = record.Chinese;
			s.bEnglish = false;
			s.Base = record;
			s.ids.push_back(idx);
			AllIndex.insert(make_pair(record.Chinese, s));
		}
		else
		{
			AllIndex[record.Chinese].ids.push_back(idx);
		}
	}
}
/*
vector<int> DictionaryManager::FindVocabularyByEnglish(string English)
{
	map<string, vector<int>>::iterator result = VocabularyEnglishIdx.find(English);
	if (VocabularyEnglishIdx.end() != result)
	{
		return result->second;
	}

	return vector<int>();
}

vector<int> DictionaryManager::FindVocabularyByChinese(string Chinese)
{
	map<string, vector<int>>::iterator result = VocabularyChineseIdx.find(Chinese);
	if (VocabularyChineseIdx.end() != result)
	{
		return result->second;
	}

	return vector<int>();
}

vector<int> DictionaryManager::FindLikeVocabularyByEnglish(string English)
{
	vector<int> result;
	size_t SearchSize = English.size();
	for (auto e : VocabularyEnglishIdx)
	{
		string key = e.first;
		if (key.size() < SearchSize)
			continue;

		string shortKey = key.substr(0, SearchSize);
		if(shortKey == English)
		{
			for (auto i : e.second)
			{
				result.push_back(i);
			}
		}
	}
	return result;
}

vector<int> DictionaryManager::FindLikeVocabularyByChinese(string Chinese)
{
	vector<int> result;
	size_t SearchSize = Chinese.size();
	for (auto e : VocabularyChineseIdx)
	{
		string key = e.first;
		if (key.size() < SearchSize)
			continue;

		string shortKey = key.substr(0, SearchSize);
		if (shortKey == Chinese)
		{
			for (auto i : e.second)
			{
				result.push_back(i);
			}
		}
	}
	return result;
}

vector<int> DictionaryManager::FindAgencyByAbbrName(string AbbrName)
{
	map<string, vector<int>>::iterator result = AgencyAbbrIdx.find(AbbrName);
	if (AgencyAbbrIdx.end() != result)
	{
		return result->second;
	}

	return vector<int>();
}

vector<int> DictionaryManager::FindAgencyByChineseName(string AgencyName)
{
	map<string, vector<int>>::iterator result = AgencyChineseIdx.find(AgencyName);
	if (AgencyChineseIdx.end() != result)
	{
		return result->second;
	}
	return vector<int>();
}

vector<int> DictionaryManager::FindLikeAgencyByAbbrName(string AbbrName)
{
	vector<int> result;
	size_t SearchSize = AbbrName.size();
	for (auto e : AgencyAbbrIdx)
	{
		string key = e.first;
		if (key.size() < SearchSize)
			continue;

		string shortKey = key.substr(0, SearchSize);
		if (shortKey == AbbrName)
		{
			for (auto i : e.second)
			{
				result.push_back(i);
			}
		}
	}
	return result;
}

vector<int> DictionaryManager::FindLikeAgencyByChineseName(string ChineseName)
{
	vector<int> result;
	size_t SearchSize = ChineseName.size();
	for (auto e : AgencyChineseIdx)
	{
		string key = e.first;
		if (key.size() < SearchSize)
			continue;

		string shortKey = key.substr(0, SearchSize);
		if (shortKey == ChineseName)
		{
			for (auto i : e.second)
			{
				result.push_back(i);
			}
		}
	}
	return result;
}

vector<int> DictionaryManager::FindGroupByAbbrName(string AbbrName)
{
	map<string, vector<int>>::iterator result = GroupAbbrIdx.find(AbbrName);
	if (GroupAbbrIdx.end() != result)
	{
		return result->second;
	}

	return vector<int>();
}

vector<int> DictionaryManager::FindGroupByChinese(string ChineseName)
{
	map<string, vector<int>>::iterator result = GroupChineseIdx.find(ChineseName);
	if (GroupChineseIdx.end() != result)
	{
		return result->second;
	}
	return vector<int>();
}

vector<int> DictionaryManager::FindLikeGroupByAbbrName(string AbbrName)
{
	vector<int> result;
	size_t SearchSize = AbbrName.size();
	for (auto e : GroupAbbrIdx)
	{
		string key = e.first;
		if (key.size() < SearchSize)
			continue;

		string shortKey = key.substr(0, SearchSize);
		if (shortKey == AbbrName)
		{
			for (auto i : e.second)
			{
				result.push_back(i);
			}
		}
	}
	return result;
}

vector<int> DictionaryManager::FindLikeGroupByChineseName(string ChineseName)
{
	vector<int> result;
	size_t SearchSize = ChineseName.size();
	for (auto e : GroupChineseIdx)
	{
		string key = e.first;
		if (key.size() < SearchSize)
			continue;

		string shortKey = key.substr(0, SearchSize);
		if (shortKey == ChineseName)
		{
			for (auto i : e.second)
			{
				result.push_back(i);
			}
		}
	}
	return result;
}
*/

const SVocabulary& DictionaryManager::GetVocabularyInfo(int Id)
{
	return AllVocabulary[Id];
}

const SAgency& DictionaryManager::GetAgencyInfo(int Id)
{
	return AllAgency[Id];
}

const SCountryGroup& DictionaryManager::GetGroupInfo(int Id)
{
	return AllGroup[Id];
}

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#include "iconv.h"

int DictionaryManager::code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;

	if (-1 == iconv(cd, (char**)pin, &inlen, pout, &outlen))
	{
		iconv_close(cd);
		return -1;
	}

	iconv_close(cd);
	return 0;
}

/*GBK转UTF8*/
std::string DictionaryManager::GBK2Utf8(const char *inbuf)
{
	if (NULL == inbuf || 0 == inbuf[0])
		return "";
	size_t inlen = strlen(inbuf);

#if(CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	char * outbuf = new char[inlen * 4 + 4];
	std::string strRet;
	if (code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, inlen * 4 + 4) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
	return strRet;
#else
	int size = inlen * 4 + 4;
	char outbuf[size];

	if (code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, size) == 0)
	{
		return string(outbuf, size);
	}
	else {
		return string();
	}
#endif
}
std::string DictionaryManager::Utf8ToGBK(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 4 + 4];
	std::string strRet;
	if (code_convert("UTF-8", "GBK", inbuf, inlen, outbuf, inlen * 4 + 4) == 0)
	{
		strRet = outbuf;
	}
	delete[] outbuf;
	return strRet;
}

#else

#include "windows.h"
//#include "iconv\iconv.h"
void UTF8ToUnicode(wchar_t* pOut, char *pText)
{
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
}
void UnicodeToUTF8(char* pOut, wchar_t* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后 
	char* pchar = (char *)pText;
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
}
void UnicodeToGB2312(char* pOut, wchar_t uData)
{
	WideCharToMultiByte(CP_ACP, NULL, &uData, 1, pOut, sizeof(wchar_t), NULL, NULL);
}
void Gb2312ToUnicode(wchar_t* pOut, char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
}

void GB2312ToUTF8(std::string& pOut, char *pText, int pLen)
{
	char buf[4] = { 0 };
	int nLength = pLen * 3;
	char* rst = new char[nLength];
	memset(rst, 0, nLength);
	int i = 0, j = 0;
	while (i < pLen)
	{
		//如果是英文直接复制就可以 
		if (*(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;
			Gb2312ToUnicode(&pbuffer, pText + i);
			UnicodeToUTF8(buf, &pbuffer);
			rst[j] = buf[0];
			rst[j + 1] = buf[1];
			rst[j + 2] = buf[2];
			j += 3;
			i += 2;
		}
	}

	//rst[j] ='\n';  	//返回结果  
	pOut = rst;
	delete[]rst;
	return;
}

void UTF_8ToGB2312(char*pOut, char *pText, int pLen)
{
	char Ctemp[4];
	memset(Ctemp, 0, 4);
	int i = 0, j = 0;
	while (i < pLen)
	{
		if (pText[i] >= 0)
		{
			pOut[j++] = pText[i++];
		}
		else
		{
			WCHAR Wtemp;
			UTF8ToUnicode(&Wtemp, pText + i);
			UnicodeToGB2312(Ctemp, Wtemp);
			pOut[j] = Ctemp[0];
			pOut[j + 1] = Ctemp[1];
			i += 3;
			j += 2;
		}
	}
	pOut[j] = '\n';
	return;
}

std::string DictionaryManager::GBK2Utf8(const char *inbuf)
{
	if (NULL == inbuf || 0 == inbuf[0])
		return "";

	std::string str;
	GB2312ToUTF8(str, (char*)inbuf, strlen(inbuf));
	return str;
}
std::string DictionaryManager::Utf8ToGBK(const char *inbuf)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

#endif