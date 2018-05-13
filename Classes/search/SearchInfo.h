#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace MySearch
{

// ����������С
#define TEXT_INPUT_FONTSIZE 16
// �����������С
#define TEXT_RESULT_FONTSIZE 14
// �ı��и߶�
#define TEXT_LINE_HEIGHT (TEXT_RESULT_FONTSIZE + 2)

// ��¼����
enum EType
{
	Agency,
	Vocabulary,
	Group,
};

// ��Ԫ������Ϣ
struct RecordBase
{
	// ����ID
	int id;
	// Ӣ�Ĵʻ㡢��д
	string English;
	// ���Ĵʻ㡢���ҡ���֯
	string Chinese;
};

// �����Ͽɻ�����¼
struct SAgency
{
	// ��������
	RecordBase Base;
	// Ӣ��ȫ��
	string EnglishFullName;
	// ��������
	string ChineseFullName;
};

// �Ͽɴʻ�
struct SVocabulary
{
	// ��������
	RecordBase Base;
	// ˵�������塢���ԡ���������ȣ�
	string Paraphrase;
	// Ӣ����Դ
	string EnglishSource;
	// Ӣ�������
	string EnglishNumber;
	// ������Դ
	string ChineseSource;
	// ���������
	string ChineseNumber;
};

// ���ʣ����ң���֯
struct SCountryGroup
{
	// ��������
	RecordBase Base;
	// Ӣ��ȫ��
	string EnglishFullName;
	// ��������
	string ChineseFullName;
};

// ��������
struct SIndexBase
{
	int Id;
	EType Type;
};

// ������Ϣ����
struct SSimpleIndex
{
	// ���ƵĽ��
	string Index;
	// �Ƿ�����Ӣ��
	bool bEnglish;
	// ������Ϣ
	RecordBase Base;
	// �����
	vector<SIndexBase> ids;
};

// ��Ϣ����
class DictionaryManager
{
public:
	DictionaryManager();
	~DictionaryManager();

public:
	// ����ԭʼ�����ļ�
	void LoadData();
	// ����ȫ��
	const SSimpleIndex* FindAll(string input);
	// ģ������ȫ��
	vector<SSimpleIndex> FindLikeAll(string input);
	// �����������
	const SSimpleIndex* GetSearchIndex(string input);
	/*
	// �����Ͽɴʻ�
	vector<SVocabulary> SearchVocabulary(string input);
	// ���������Ͽɻ���
	vector<SAgency> SearchAgency(string input);
	// �������ʣ����ң���֯
	vector<SCountryGroup> SearchGroup(string input);
	*/
	const SVocabulary& GetVocabularyInfo(int Id);
	const SAgency& GetAgencyInfo(int Id);
	const SCountryGroup& GetGroupInfo(int Id);
/*
	// ��ȷ����
protected:
	// ͨ��Ӣ�������Ͽɴʻ�
	vector<int> FindVocabularyByEnglish(string English);
	// ͨ�����������Ͽɴʻ�
	vector<int> FindVocabularyByChinese(string Chinese);
	// ͨ��Ӣ����д���������Ͽɻ���
	vector<int> FindAgencyByAbbrName(string AbbrName);
	// ͨ���������������Ͽɻ���
	vector<int> FindAgencyByChineseName(string AgencyName);
	// ͨ��Ӣ����д�������ʣ����ң���֯
	vector<int> FindGroupByAbbrName(string AbbrName);
	// ͨ�������������ʣ����ң���֯
	vector<int> FindGroupByChinese(string ChineseName);

	// ģ������
protected:
	// ͨ��Ӣ��ģ�������Ͽɴʻ�
	vector<int> FindLikeVocabularyByEnglish(string English);
	// ͨ������ģ�������Ͽɴʻ�
	vector<int> FindLikeVocabularyByChinese(string Chinese);

	// ͨ��Ӣ����дģ�����������Ͽɻ���
	vector<int> FindLikeAgencyByAbbrName(string AbbrName);
	// ͨ������ģ�����������Ͽɻ���
	vector<int> FindLikeAgencyByChineseName(string ChineseName);

	// ͨ��Ӣ����дģ���������ʣ����ң���֯
	vector<int> FindLikeGroupByAbbrName(string AbbrName);
	// ͨ������ģ���������ʣ����ң���֯
	vector<int> FindLikeGroupByChineseName(string ChineseName);
	*/
	// ������Դ
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
	// ���е�רҵ�ʻ��б�
	map<int, SVocabulary> AllVocabulary;
// 	// Ӣ�Ĵʻ�����
// 	map<string, vector<int>> VocabularyEnglishIdx;
// 	// ��������
// 	map<string, vector<int>> VocabularyChineseIdx;
	// �����Ͽɻ�����¼
	map<int, SAgency> AllAgency;
// 	// �����Ͽɻ�����¼ ��������
// 	map<string, vector<int>> AgencyChineseIdx;
// 	// �����Ͽɻ�����¼ Ӣ����д����
// 	map<string, vector<int>> AgencyAbbrIdx;

	// ���ʣ����ң���֯
	map<int, SCountryGroup> AllGroup;
// 	// ���ʣ����ң���֯ ��������
// 	map<string, vector<int>> GroupChineseIdx;
// 	// ���ʣ����ң���֯ Ӣ����д����
// 	map<string, vector<int>> GroupAbbrIdx;

	// ���еĴ�����
	map<string, SSimpleIndex> AllIndex;

	// ������һ�εĲ���
	vector<SSimpleIndex> CacheSearchResult;
	// ��һ�β��ҵ��ַ���
	string SaveLastIndex;
};

extern DictionaryManager g_Dictionary;

}
