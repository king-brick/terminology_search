#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace MySearch
{

// 输入框字体大小
#define TEXT_INPUT_FONTSIZE 14
// 输出结果字体大小
#define TEXT_RESULT_FONTSIZE 14
// 文本行高度
#define TEXT_LINE_HEIGHT (TEXT_RESULT_FONTSIZE + 2)

// 输入框的字体大小
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define INPUT_BOX_HEIGHT (TEXT_INPUT_FONTSIZE + 10)
#else
#define INPUT_BOX_HEIGHT (TEXT_INPUT_FONTSIZE + 4)
#endif

// 查找时每帧add到listview的item数量
#define ADD_ITEM_PERFRAME 10

// 名录类型
enum EType
{
	Agency,
	Vocabulary,
	Group,
};

// 单元基础信息
struct RecordBase
{
	// 索引ID
	int id;
	// 英文词汇、缩写
	string English;
	// 中文词汇、国家、组织
	string Chinese;
};

// 国家认可机构名录
struct SAgency
{
	// 基础数据
	RecordBase Base;
	// 英文全称
	string EnglishFullName;
	// 中文译名
	string ChineseFullName;
};

// 认可词汇
struct SVocabulary
{
	// 基础数据
	RecordBase Base;
	// 说明（释义、词性、适用领域等）
	string Paraphrase;
	// 英文来源
	string EnglishSource;
	// 英文条款号
	string EnglishNumber;
	// 中文来源
	string ChineseSource;
	// 中文条款号
	string ChineseNumber;
};

// 国际（国家）组织
struct SCountryGroup
{
	// 基础数据
	RecordBase Base;
	// 英文全称
	string EnglishFullName;
	// 中文译名
	string ChineseFullName;
};

// 基础索引
struct SIndexBase
{
	int Id;
	EType Type;
};

// 基础信息索引
struct SSimpleIndex
{
	// 类似的结果
	string Index;
	// 是否搜索英文
	bool bEnglish;
	// 基础信息
	RecordBase Base;
	// 多义词
	vector<SIndexBase> ids;
};

// 信息管理
class DictionaryManager
{
public:
	DictionaryManager();
	~DictionaryManager();

public:
	// 加载原始数据文件
	void LoadData();
	// 搜索全部
	const SSimpleIndex* FindAll(string input);
	// 模糊查找全部
	vector<SSimpleIndex> FindLikeAll(string input);
	// 返回搜索结果
	const SSimpleIndex* GetSearchIndex(string input);
	/*
	// 搜索认可词汇
	vector<SVocabulary> SearchVocabulary(string input);
	// 搜索国家认可机构
	vector<SAgency> SearchAgency(string input);
	// 搜索国际（国家）组织
	vector<SCountryGroup> SearchGroup(string input);
	*/
	const SVocabulary& GetVocabularyInfo(int Id);
	const SAgency& GetAgencyInfo(int Id);
	const SCountryGroup& GetGroupInfo(int Id);
/*
	// 精确查找
protected:
	// 通过英文搜索认可词汇
	vector<int> FindVocabularyByEnglish(string English);
	// 通过中文搜索认可词汇
	vector<int> FindVocabularyByChinese(string Chinese);
	// 通过英文缩写搜索国家认可机构
	vector<int> FindAgencyByAbbrName(string AbbrName);
	// 通过中文搜索国家认可机构
	vector<int> FindAgencyByChineseName(string AgencyName);
	// 通过英文缩写搜索国际（国家）组织
	vector<int> FindGroupByAbbrName(string AbbrName);
	// 通过中文搜索国际（国家）组织
	vector<int> FindGroupByChinese(string ChineseName);

	// 模糊查找
protected:
	// 通过英文模糊搜索认可词汇
	vector<int> FindLikeVocabularyByEnglish(string English);
	// 通过中文模糊搜索认可词汇
	vector<int> FindLikeVocabularyByChinese(string Chinese);

	// 通过英文缩写模糊搜索国家认可机构
	vector<int> FindLikeAgencyByAbbrName(string AbbrName);
	// 通过中文模糊搜索国家认可机构
	vector<int> FindLikeAgencyByChineseName(string ChineseName);

	// 通过英文缩写模糊搜索国际（国家）组织
	vector<int> FindLikeGroupByAbbrName(string AbbrName);
	// 通过中文模糊搜索国际（国家）组织
	vector<int> FindLikeGroupByChineseName(string ChineseName);
	*/
	// 加载资源
protected:
	void LoadVocabulary();
	void LoadAgency();
	void LoadGroup();
	void Add2AllIndex(const RecordBase& record, EType type);

public:
	static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);
	static string GBK2Utf8(const char *inbuf);
	static string Utf8ToGBK(const char *inbuf);

private:
	// 所有的专业词汇列表
	map<int, SVocabulary> AllVocabulary;
// 	// 英文词汇索引
// 	map<string, vector<int>> VocabularyEnglishIdx;
// 	// 中文索引
// 	map<string, vector<int>> VocabularyChineseIdx;
	// 国家认可机构名录
	map<int, SAgency> AllAgency;
// 	// 国家认可机构名录 中文索引
// 	map<string, vector<int>> AgencyChineseIdx;
// 	// 国家认可机构名录 英文缩写索引
// 	map<string, vector<int>> AgencyAbbrIdx;

	// 国际（国家）组织
	map<int, SCountryGroup> AllGroup;
// 	// 国际（国家）组织 中文索引
// 	map<string, vector<int>> GroupChineseIdx;
// 	// 国际（国家）组织 英文缩写索引
// 	map<string, vector<int>> GroupAbbrIdx;

	// 所有的词索引
	map<string, SSimpleIndex> AllIndex;

	// 缓存上一次的查找
	vector<SSimpleIndex> CacheSearchResult;
	// 上一次查找的字符数
	string SaveLastIndex;
};

extern DictionaryManager g_Dictionary;

}
