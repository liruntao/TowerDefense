/*
�򵥵�һ��csv����
*/

#include "CsvData.h"

bool CsvData::init()
{
    return true;
}


//����������ɣ�ֻ������һ��valuevector��pushback��˭������Ҳ����ʼ����??
//����Ӧ�ü���ȡm_allLinesVector=һ����ǰ�洢���ļ������ã���util�load��ʱ���ȫ�����ȥ�ˡ�
//û�����⣬����һ��������˭������������ʱ����һ���ֲ��������ȵ�������node�ͷ��ˣ����ͻᱻ�ͷ�
//addlinedata��ʱ�򣬴����linedata���Ѿ���util�б�������ˣ����ǲ�����������һ�����ݣ�������һ��ʵ����������ԡ�
//ʹ�õ�ʱ�����������ǰ�˳���ŵģ����ֱ�ӵ��ø��е����ݼ��ɡ�
void CsvData::addLineData(ValueVector lineData)
{
    m_allLinesVec.push_back(Value(lineData));
}

cocos2d::ValueVector CsvData::getSingleLineData(int iLine)
{
    return m_allLinesVec.at(iLine).asValueVector();
}

Size CsvData::getRowColNum()
{
    Size size = Size();

    size.width = m_allLinesVec.size();

    if (size.width > 0) {
        size.height = m_allLinesVec.at(0).asValueVector().size();
    }

    return size;
}
