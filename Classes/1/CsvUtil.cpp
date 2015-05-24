/*
mcsvmap<csvfilepath,csvdata>就是一个valuevector和它所对应的存储路径。
csvdata<valueVector<每一行的数据，是一个value>>
这里只读，只进行操作，但是不存，存储的时候一般都是使用string存储的。
string与value的一个转换，value的数据操作起来比string要方便的多。
*/


#include "CsvUtil.h"
#include "StringUtil.h"

CsvUtil* CsvUtil::m_CsvUtil = NULL;
//工具类应该都有这个函数，getinstance。
//而且工具类一般都没有析构函数，释放也是随着调用它的entity释放而释放。
CsvUtil* CsvUtil::getInstance(){
    if(m_CsvUtil == NULL) {
        m_CsvUtil = new CsvUtil();
        if(m_CsvUtil && m_CsvUtil->init()) {
            m_CsvUtil->autorelease();
            m_CsvUtil->retain();
        }
        else {
            CC_SAFE_DELETE(m_CsvUtil);
            m_CsvUtil = NULL;
        }
    }

    return m_CsvUtil;
}

bool CsvUtil::init(){
    return true;
}

void CsvUtil::loadFile( const char* sPath ){

    /* 存放一个csv文件的对象。 */
    CsvData* csvData = CsvData::create();

    /* 读取数据，按行保存到列表中
	保存的时候都按字符串保存，就行逻辑运算的时候，用value或vector？
	*/
    std::string str = FileUtils::getInstance()->getStringFromFile(sPath);
    ValueVector linesList = StringUtil::getInstance()->split(str.c_str(), "\n");

    /* 把每一行的字符串拆分出来（按逗号分隔） */
    for(auto value : linesList) {
        /* 
            将一行的字符串按逗号分隔，然后存放到列表中，最后将列表存放到CsvData对象里。
            换句话说，csvData将成为一个二维表格，记录了配置文件行和列的字符串
        */
        ValueVector tArr = StringUtil::getInstance()->split(value.asString().c_str(), ",");
        csvData->addLineData(tArr);
    }

    /* 添加列表到字典里 */
    mCsvMap.insert(sPath, csvData);
}

Value CsvUtil::getValue( int iRow, int iCol, const char* csvFilePath ) {
    /* 取出Csv文件对象 */
    auto csvData = mCsvMap.at(csvFilePath);

    /* 如果配置文件的数据不存在，则加载配置文件
	//这样每次调用都加载，就是加载到内存中，固然可以加快运算速度，但是都会占用太大的内存？？*/
    if(csvData == nullptr) {
        loadFile(csvFilePath);
        csvData = mCsvMap.at(csvFilePath);
    }
    Size size = csvData->getRowColNum();

    /* 数据行数
	以下有一个容错机制*/
    int iRowNum = size.width;
    int iColNum = size.height;

    /* 下标越界 */
    if(iRow >= iRowNum || iCol >= iColNum) {
        return Value("");//这里可以进行一些处理。
    }

    /* 获取第iRow行数据 */
    ValueVector rowVector = csvData->getSingleLineData(iRow);

    /* 获取第iCol列数据 */
    Value colValue = rowVector.at(iCol);

    return colValue;
}

const std::string CsvUtil::get(int iRow, int iCol, const char* csvFilePath) {
    Value colValue = getValue(iRow, iCol, csvFilePath);

    return colValue.asString();//返回一个astring的数据，操作同getvalue，其他类型同，可以统一搞一个函数，返回各种类型的数据。
}

const Size CsvUtil::getFileRowColNum( const char* csvFilePath )
{
    /* 取出配置文件的二维表格 */
    auto csvData = mCsvMap.at(csvFilePath);

    /* 如果配置文件的数据不存在，则加载配置文件 */
    if(csvData == nullptr) {
        loadFile(csvFilePath);
        csvData = mCsvMap.at(csvFilePath);
    }
    Size size = csvData->getRowColNum();
    return size;
}

const int CsvUtil::getInt( int iRow, int iCol, const char* csvFilePath ) {
    Value colValue = getValue(iRow, iCol, csvFilePath);
	return colValue.asInt();
}

const float CsvUtil::getFloat(int iRow, int iCol, const char* csvFilePath) {
    Value colValue = getValue(iRow, iCol, csvFilePath);

    return colValue.asFloat();
}

const bool CsvUtil::getBool(int iRow, int iCol, const char* csvFilePath)
{
    Value colValue = getValue(iRow, iCol, csvFilePath);
    return colValue.asBool();
}
//根据id的值返回它所在的行
const int CsvUtil::findValueInWithLine( const char* chValue, int iValueCol, const char* csvFilePath ) {
    Size csvSize = getFileRowColNum(csvFilePath);

    int iLine = -1;
    for(int i = 2; i < csvSize.width; i++) {
        int ID = getInt(i, iValueCol, csvFilePath);
        if(Value(ID).asString().compare(chValue) == 0) {
            iLine = i;
            break;
        }
    }

    return iLine;
}
