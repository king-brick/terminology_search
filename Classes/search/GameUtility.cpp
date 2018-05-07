#include "GameUtility.h"
#include "md5.h"
#include <time.h>
#include "Macros.h"
#include "TableHelper.h"
#include "GameDefine.h"

/**-------------------------------------------------------------------------------
	[功能]：得到客户端距离1900年到现在的秒数
	[返回]：
*/
int getCurrentClientTime()
{
					// 不同平台下，得到秒数，再将秒数转换成日期的格式

					time_t tmSecond;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
					// 取得秒数
					time(&tmSecond);  
#else
					// 取得秒数
					cc_timeval now;  
					CCTime::gettimeofdayCocos2d(&now, NULL);				
					tmSecond = now.tv_sec;  
#endif 
					return tmSecond;
}

/**-------------------------------------------------------------------------------
	[功能]：通过秒数得到详细的时间：年月日天时分秒
*/
void getDetailTime( int nSecondCount, int *year, int *month, int *day, int *hour, int *minute, int *second )
{
	if(nSecondCount<0) return ;

	time_t tmSecond = nSecondCount;
	// 秒数转换成为标准时间格式
	tm *tmFormat;	
	tmFormat = localtime( &tmSecond );  
	year!=NULL ? *year = tmFormat->tm_year + 1900 : NULL; 
	month!=NULL? *month = tmFormat->tm_mon + 1: NULL; 
	day!=NULL? *day = tmFormat->tm_mday: NULL; 
	hour!=NULL? *hour = tmFormat->tm_hour: NULL;  
	minute!=NULL? *minute = tmFormat->tm_min: NULL; 
	second!=NULL? *second = tmFormat->tm_sec: NULL; 
}

bool isInSameDay(int nTime1, int nTime2)
{
	tm *tmFormat1, *tmFormat2;	
	time_t t1 = nTime1, t2 = nTime2;

	tmFormat1 = localtime(&t1);
	int year1 = tmFormat1->tm_year;
	int month1 = tmFormat1->tm_mon;
	int day1 = tmFormat1->tm_mday;

	tmFormat2 = localtime(&t2);  
	int year2 = tmFormat1->tm_year;
	int month2 = tmFormat1->tm_mon;
	int day2 = tmFormat1->tm_mday;

	return (year1 == year2 && month1 == month2 && day1 == day2);
}

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#include "iconv.h"

int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    iconv_t cd;
    const char *temp = inbuf;
    const char **pin = &temp;
    char **pout = &outbuf;
    memset(outbuf,0,outlen);
    cd = iconv_open(to_charset,from_charset);
    if(cd==0)
        return -1;
    
    if(-1 == iconv(cd,(char**)pin,&inlen,pout,&outlen))
    {
		iconv_close(cd);
		return -1;
	}

    iconv_close(cd);
    return 0;
}

/*GBK转UTF8*/
std::string GBK2Utf8(const char *inbuf)
{
	if(NULL == inbuf || 0 == inbuf[0])
		return "";
    size_t inlen = strlen(inbuf);

#if(CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    char * outbuf = new char[inlen * 4 + 4];
    std::string strRet;
    if(code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, inlen * 4 + 4) == 0)
    {
        strRet = outbuf;
    }
    delete [] outbuf;
    return strRet;
#else
    int size = inlen*4 +4;
    char outbuf[size];

	if(code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, size) == 0)
	{
		return string(outbuf,size);
	}
	else{
		return string();
	}
#endif
}
std::string Utf8ToGBK(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 4 + 4];
	std::string strRet;
	if(code_convert("UTF-8", "GBK", inbuf, inlen, outbuf, inlen * 4 + 4) == 0)
	{
		strRet = outbuf;
	}
	delete [] outbuf;
	return strRet;
}

#else

#include "windows.h"
#include "iconv\iconv.h"


void UTF8ToUnicode(wchar_t* pOut,char *pText)
{   
	char* uchar = (char *)pOut; 
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F); 
} 
void UnicodeToUTF8(char* pOut,wchar_t* pText) 
{ 
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后 
	char* pchar = (char *)pText; 
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
} 
void UnicodeToGB2312(char* pOut,wchar_t uData)
{ 
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL); 
}
void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer) 
{   
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
}

void GB2312ToUTF8(std::string& pOut, char *pText, int pLen)
{  
	char buf[4] = {0}; 
	int nLength = pLen* 3; 
	char* rst = new char[nLength]; 	
	memset(rst,0,nLength);  
	int i = 0 ,j = 0; 
	while(i < pLen) 
	{ 
		//如果是英文直接复制就可以 
		if( *(pText + i) >= 0) 
		{  
			rst[j++] = pText[i++];
		} 
		else  
		{  
			wchar_t pbuffer;  
			Gb2312ToUnicode(&pbuffer,pText+i);
			UnicodeToUTF8(buf,&pbuffer);
			rst[j] = buf[0];   
			rst[j+1] = buf[1]; 
			rst[j+2] = buf[2]; 
			j += 3; 
			i += 2; 
		}   
	}  

	//rst[j] ='\n';  	//返回结果  
	pOut = rst;
	delete []rst;
	return;   
}

void UTF_8ToGB2312(char*pOut, char *pText, int pLen)  
{   
	char Ctemp[4];   
	memset(Ctemp,0,4); 
	int i =0 ,j = 0; 
	while(i < pLen) 
	{  
		if(pText[i] >= 0)  
		{  
			pOut[j++] = pText[i++]; 
		} 
		else 
		{  
			WCHAR Wtemp; 
			UTF8ToUnicode(&Wtemp,pText + i);
			UnicodeToGB2312(Ctemp,Wtemp); 
			pOut[j] = Ctemp[0];
			pOut[j + 1] = Ctemp[1];  
			i += 3;   
			j += 2;  
		}   
	} 
	pOut[j] ='\n'; 
	return; 
} 

std::string GBK2Utf8(const char *inbuf)
{
	if(NULL == inbuf || 0 == inbuf[0])
		return "";

	std::string str;
	GB2312ToUTF8(str, (char*)inbuf, strlen(inbuf));
	return str;
}
std::string Utf8ToGBK(const char *inbuf)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

#endif

class PageTransitionBackward : public CCTransitionPageTurn
{
public:
	static CCTransitionScene* create(float t, CCScene* s)
	{
		CCDirector::sharedDirector()->setDepthTest(true);
		return CCTransitionPageTurn::create(t, s, true);
	}
};

class PageTransitionForward : public CCTransitionPageTurn
{
public:
	static CCTransitionScene* create(float t, CCScene* s)
	{
		CCDirector::sharedDirector()->setDepthTest(true);
		return CCTransitionPageTurn::create(t, s, false);
	}
};

class FlipAngularRightOver : public CCTransitionFlipAngular 
{
public:
	static CCTransitionScene* create(float t, CCScene* s)
	{
		return CCTransitionFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
	}
};

string md5Pwd( const string& pwd )
{
	auto prefix = MD5("%d").ToString();
	prefix += "\\.";
	prefix = MD5(prefix.c_str()).ToString();
	prefix += "#a2";
	prefix = MD5(prefix.c_str()).ToString();
	auto suffix = MD5("%02x").ToString();
	suffix += "$01";
	suffix = MD5(prefix.c_str()).ToString();

	auto finalPwd = prefix + pwd + suffix;
	return MD5(finalPwd.c_str()).ToString();
}

void ReplaceScene( CCScene* pScene,float dt )
{
	CCDirector::sharedDirector()->replaceScene(pScene);
	return;
	//获得随机种子
	srand((unsigned)time(0));
	int index=rand()%16;
	switch (index)
	{
	case 0:
		CCDirector::sharedDirector()->replaceScene(CCTransitionFadeTR::create(0.5f, pScene));
		break;
	case 1:
		CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(0.5f, pScene));
		break;
	case 2:
		CCDirector::sharedDirector()->replaceScene(CCTransitionFadeUp::create(0.5f, pScene));
		break;
	case 3:
		CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f, pScene));
		break;
	case 4:
		CCDirector::sharedDirector()->replaceScene(CCTransitionSplitCols::create(0.5f, pScene));
		break;
	case 5:
		CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(0.5f, pScene));
		break;
	case 6:
		CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::create(0.5f, pScene));
		break;
	case 7:
		CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(0.5f, pScene));
		break;
	case 8:
		CCDirector::sharedDirector()->replaceScene(PageTransitionBackward::create(0.5f, pScene));	
		break;
	case 9:
		CCDirector::sharedDirector()->replaceScene(CCTransitionProgressRadialCCW::create(0.5f, pScene));	
		break;
	case 10:
		CCDirector::sharedDirector()->replaceScene(CCTransitionProgressRadialCW::create(0.5f, pScene));	
		break;
	case 11:
		CCDirector::sharedDirector()->replaceScene(CCTransitionProgressInOut::create(0.5f, pScene));	
		break;
	case 12:
		CCDirector::sharedDirector()->replaceScene(CCTransitionProgressOutIn::create(0.5f, pScene));	
		break;
	case 13:
		CCDirector::sharedDirector()->replaceScene(PageTransitionForward::create(0.5f, pScene));	
		break;
	case 14:
		CCDirector::sharedDirector()->replaceScene(FlipAngularRightOver::create(0.5f, pScene));	
		break;
	case 15:
		CCDirector::sharedDirector()->replaceScene(CCTransitionShrinkGrow::create(0.5f, pScene));	
		break;
	}
}

// 在字符串中每个字符后面添加一个字符（汉字为1个字符）
void AddCharAfterOneChar(const std::string &src, char cAdd, std::string &result)
{
	char ch;
	for (size_t i = 0; i < src.length(); ++i)
	{
		ch = src.at(i);
		result += ch;
		if (ch & 0x1000)
		{ // 当前字符是汉字(汉字默认是有两个assic码构成)
			result += src.at(++i);
		}

		result += cAdd;
	}
}

bool IsEmptyString(const char* info)
{
	return strcmp(info, "") == 0;
}

bool IsHaveIllegalChar(const char* info)
{
	//检测字符串是否有非法字符
	string strOutput;
	if(TableHelper::strFilter(info, strOutput, StrFilterType::FILTER_CREATE_CHAR))
	{
		return true;
	}
	return false;
}

cocos2d::CCSprite* maskedSpriteWithSprite(cocos2d::CCSprite* textureSprite, cocos2d::CCSprite* maskSprite)  
{  
	// 1   
	int w = maskSprite->getContentSize().width * maskSprite->getScaleX();  
	int h = maskSprite->getContentSize().height * maskSprite->getScaleY();  
	CCRenderTexture* rt = CCRenderTexture::create(w, h);  

	// 2   
	maskSprite->setPosition( ccp(maskSprite->getContentSize().width *  maskSprite->getScaleX()/2,   
		maskSprite->getContentSize().height * maskSprite->getScaleY()/2));  
	textureSprite->setPosition( ccp(textureSprite->getContentSize().width *  textureSprite->getScaleX() /2,   
		textureSprite->getContentSize().height * textureSprite->getScaleY()/2));  

	// 3   
	ccBlendFunc blendFunc;  
	blendFunc.src = GL_ONE;  
	blendFunc.dst = GL_ZERO;  
	maskSprite->setBlendFunc(blendFunc);  

	blendFunc.src = GL_DST_ALPHA;            // mask图片的当前alpha值是多少，如果是0（完全透明），那么就显示mask的。如果是1（完全不透明）   
	blendFunc.dst = GL_ZERO;                // maskSprite不可见   
	textureSprite->setBlendFunc(blendFunc);  

	// 4   
	rt->begin();  
	maskSprite->visit();  
	textureSprite->visit();  
	rt->end();  

	// 5   
	CCSprite* retval = CCSprite::createWithTexture(rt->getSprite()->getTexture());  
	retval->setFlipY(true);  
	return retval;  
}  

bool screenShot(CCNode* pNode, const string& sFileName)
{
	int w = pNode->getContentSize().width * pNode->getScaleX();  
	int h = pNode->getContentSize().height * pNode->getScaleY(); 
	CCRenderTexture* rt = CCRenderTexture::create(w, h); 
	CCPoint ptOriginalPos = pNode->getPosition();
	CCPoint ptOriginalAnchorPoint = pNode->getAnchorPoint();
	bool bVisible = pNode->isVisible();
	pNode->setPosition(CCPointZero);  
	pNode->setAnchorPoint(CCPointZero);
	pNode->setVisible(true);

	rt->begin();
	pNode->visit();
	rt->end();

	pNode->setPosition(ptOriginalPos);  
	pNode->setAnchorPoint(ptOriginalAnchorPoint);
	pNode->setVisible(bVisible);

	return rt->saveToFile(sFileName.c_str(), kCCImageFormatPNG);
}

CCSprite* getRenderTexture(CCNode* node)
{
	assert(node != NULL);
	if (node == NULL)
		return NULL;

	auto pGLView = CCDirector::sharedDirector()->getOpenGLView();

	auto size = node->getContentSize();
	CCRenderTexture* renderTexture = CCRenderTexture::create(size.width, size.height, 
		kCCTexture2DPixelFormat_RGBA8888, GL_DEPTH24_STENCIL8);
	renderTexture->setAnchorPoint(CCPointZero);
	renderTexture->setPosition(CCPointZero);
	renderTexture->getSprite()->setAnchorPoint(ccp(0.5f, 0.5f));
	renderTexture->getSprite()->setPosition(ccp(size.width / 2, size.height / 2));

	float oldScaleX = pGLView->getScaleX();
	pGLView->setSScale(1.0f);
	auto oldAnchor = node->getAnchorPoint();
	auto oldPosition = node->getPosition();
	node->setAnchorPoint(CCPointZero);
	node->setPosition(CCPointZero);
	renderTexture->beginWithClear(0, 0, 0, 0, 1);
	node->visit();
	renderTexture->end();
	node->setAnchorPoint(oldAnchor);
	node->setPosition(oldPosition);
	pGLView->setSScale(oldScaleX);

	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
	renderTexture->getSprite()->setBlendFunc(blendFunc);
	renderTexture->getSprite()->setFlipX(true);

	CCSprite* retval = CCSprite::createWithTexture(renderTexture->getSprite()->getTexture());  
	retval->setFlipY(true);  
	return retval;  
}

void string_replace(std::string& strBig, const std::string & strsrc, const std::string &strdst) 
{
	std::string::size_type pos = 0;

	while( (pos = strBig.find(strsrc, pos)) != string::npos)
	{
		strBig.replace(pos, strsrc.length(), strdst);
		pos += strdst.length();
	}

}

// 把node转换为图片
cocos2d::CCSprite* createSpriteByNode(cocos2d::CCNode* pNode)
{
	int w = pNode->getContentSize().width * pNode->getScaleX();  
	int h = pNode->getContentSize().height * pNode->getScaleY(); 
	CCRenderTexture* rt = CCRenderTexture::create(w, h); 
	CCPoint ptOriginalPos = pNode->getPosition();
	CCPoint ptOriginalAnchorPoint = pNode->getAnchorPoint();
	bool bVisible = pNode->isVisible();
	pNode->setPosition(CCPointZero);  
	pNode->setAnchorPoint(CCPointZero);
	pNode->setVisible(true);

	rt->begin();
	pNode->visit();
	rt->end();

	pNode->setPosition(ptOriginalPos);  
	pNode->setAnchorPoint(ptOriginalAnchorPoint);
	pNode->setVisible(bVisible);

	CCSprite* retval = CCSprite::createWithTexture(rt->getSprite()->getTexture());  
	retval->setFlipY(true);  
	return retval;
}

BitFlag::BitFlag( int nFlagCount )
{
	m_nCount = nFlagCount;
	int intCount = nFlagCount / 32;
	if (nFlagCount % 32 > 0)
	{
		++intCount;
	}
	for (int i = 0; i != intCount; ++i)
	{
		m_Flag.push_back(-1);
	}
}

BitFlag::BitFlag( int nFlagCount, int InitValue )
{
	m_nCount = nFlagCount;
	int intCount = nFlagCount / 32;
	if (nFlagCount % 32 > 0)
	{
		++intCount;
	}
	for (int i = 0; i != intCount; ++i)
	{
		m_Flag.push_back(InitValue);
	}
}

int BitFlag::GetFlag( int nIndex )
{
	if (nIndex < 0 || nIndex >= m_nCount)
	{
		return -1;
	}
	return ((m_Flag[nIndex / 32] >> (nIndex % 32)) & 1);
}

void BitFlag::SetFlag( int nIndex )
{
	m_Flag[nIndex / 32] |= 1 << (nIndex % 32);
}

void BitFlag::CleanFlag( int nIndex )
{
	m_Flag[nIndex / 32] &= ~(1 << (nIndex % 32));
}

void BitFlag::Init(const int* pData)
{
	if(NULL == pData)
		return;

	for (int i=0; i != m_nCount/32; ++i)   
	{
		m_Flag[i]=pData[i];
	}
}

void BitFlag::ReSetAllFlag()
{
	for (int i = 0; i < (int)m_Flag.size(); i++)
	{
		m_Flag[i] = 0;
	}
}

unsigned char* getFileDataByCocos2d(const char* pszFileName, unsigned long& uSize)
{
	std::string fullPath = GETFULL_PATH(pszFileName);

	return CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &uSize);
}
void split(vector<string>& vecOutput, const string& strInput)
{
	split(vecOutput, strInput, "\t");
}

void split(vector<string>& vecOutput, const string& strInput, const char* token)
{
	int nend=0;
	int nbegin=0;
	while(nend != -1)
	{
		nend = strInput.find(token, nbegin);
		if(nend == -1)
			vecOutput.push_back(strInput.substr(nbegin, strInput.length()-nbegin));
		else
			vecOutput.push_back(strInput.substr(nbegin, nend-nbegin));
		nbegin = nend + strlen(token);
	}
}

int Convert_Int(const string& strInput)
{
	return FromString<int>(strInput);
}

float Convert_Double(const string& strInput)
{
	return FromString<float>(strInput);
}

void Convert_Value(vector<int>& vecField, const string& strInput)
{
	if(0 == strInput.size())
		return;

	vecField.clear();

	vector<string> temp;
	split(temp, strInput, ",");
	vector<string>::iterator s=temp.begin();
	for(;s!=temp.end();++s)
	{
		int temp_int = Convert_Int(*s);
		vecField.push_back(temp_int);
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void Convert_Value(vector<vector<int>>& vecField, const string& strInput)
{
	if(0 == strInput.size())
		return;
	vecField.clear();
	vecField.reserve(50);

	vector<string> temp1; temp1.reserve(20);
	split(temp1, strInput, ";");
	vector<string>::iterator ss=temp1.begin();
	int i = 0;
	for(;ss!=temp1.end();++ss)
	{
		vector<int> temp; temp.reserve(20);
		vecField.push_back(temp);	//这里是vector复制，会有性能问题
		vector<string> temp2; temp2.reserve(20);
		split(temp2,(*ss),",");				
		vector<string>::iterator s=temp2.begin();
		for(;s!=temp2.end();++s)
		{
			vecField[i].push_back(Convert_Int(*s));
		}
		++i;				
	}
}
#else
void Convert_Value(vector<vector<int>>& vecField, const string& strInput)
{
	if(0 == strInput.size())
		return;
	vecField.clear();

	vector<string> temp1;
	split(temp1, strInput, ";");
	vector<string>::iterator ss=temp1.begin();
	int i = 0;
	for(;ss!=temp1.end();++ss)
	{
		vector<int> temp;
		vecField.push_back(temp);
		vector<string> temp2;
		split(temp2,(*ss),",");
		vector<string>::iterator s=temp2.begin();
		for(;s!=temp2.end();++s)
		{
			vecField[i].push_back(Convert_Int(*s));
		}
		++i;				
	}
}
#endif


const int calculateUtf8StringLength(const char* utf8Str, int* pMultiBytesCharCount)
{
	if (utf8Str == NULL)
		return 0;

	int utf8StrLen = 0;
	const char* p = utf8Str;
	int multiBytesCharCount = 0;

	while (*p != '\0')
	{
		for (int i = 1; i <= 6; i++)
		{
			int x = i == 1 ? i : i + 1;
			if ((*p & (1 << (8 - x))) == 0)
			{
				multiBytesCharCount += i > 1;
				utf8StrLen++;
				p += i;
				break;
			}
		}
	}

	if (pMultiBytesCharCount != NULL)
		pMultiBytesCharCount[0] = multiBytesCharCount;

	return utf8StrLen;
}


//b	c	d	f	g	h	j	k	m	p	q	r	t	v	w	x	y	2	3	4	6	7	8	9
static const char sel[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'b','c','d','f','g','h','j','k','m','p',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'q','r','t','v',
	'w','x','y','2','3','4','6','7','8','9',0,0,0,0,0,0,0,0,0,0,0,0};

static const int reverse_sel[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	17,18,19,-1,20,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,-1,
	3,4,5,-1,6,7,-1,8,-1,-1,9,10,11,-1,12,-1,13,14,15,16,-1,
	-1,-1,-1,-1,-1,-1,-1,0,1,2,-1,3,4,5,-1,6,7,-1,8,-1,-1,9,
	10,11,-1,12,-1,13,14,15,16,-1};

uint64 b24toi(const char *ptr, size_t count)
{
	char* buf = (char*)ptr;
	uint64 result = reverse_sel[*buf];
	buf++;
	while(*buf)
	{
		result *= 24;
		result += reverse_sel[*buf];
		buf++;
	}

	return result;
}

const char codes[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n'};

char *itob24(uint64 str, char *buf, size_t count)
{
	char temp[8] = {0};

	int index = 0;
	while (str >=24)
	{
		temp[index++] = codes[str%24];
		str /= 24;
	}
	temp[index++] = codes[str];
	std::reverse(temp, temp + index);

	for (size_t i = 0; i < 8 && i < count; ++i)
	{
		buf[i] = sel[(int)temp[i]];
	}
	return buf;
}


static const char utf8_skip_data[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 6, 6, 1, 1
};

string utf8_substr(const string& str,unsigned int pos,int len)
{
	CCAssert(len >= 0,"");
	unsigned int startPos = 0;
	while (startPos < str.length() && pos > 0)
	{
		startPos = startPos + (unsigned int)utf8_skip_data[*((unsigned char *)(str.data() + startPos))];
		pos = pos - 1;
	}
	unsigned int endPos = startPos;
	while (endPos < str.length() && len > 0)
	{
		endPos = endPos + (unsigned int)utf8_skip_data[*((unsigned char *)(str.data() + endPos))];
		len = len - 1;
	}
	return str.substr(startPos,MIN(endPos - startPos,str.length()));

}

string utf8_substr_by_charlength(const string& str,int len)
{
	CCAssert(len >= 0,"");
	unsigned int pos = 0;
	while (pos < str.length())
	{
		unsigned int utf8CharLen = (unsigned int)utf8_skip_data[*((unsigned char *)(str.data() + pos))];
		if(pos + utf8CharLen <= (unsigned int)len)
		{
			pos = pos + utf8CharLen;
		}
		else
		{
			break;
		}
	}

	return str.substr(0,pos); 
}
