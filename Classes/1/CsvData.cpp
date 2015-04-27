/*
简单的一个csv数据
*/

#include "CsvData.h"

bool CsvData::init()
{
    return true;
}


//这里有问题吧，只传入了一个valuevector，pushback给谁？而且也不初始化。??
//这里应该加上取m_allLinesVector=一个以前存储的文件，不用，在util里，load的时候就全部存进去了。
//没有问题，就是一个单例，谁调用它，就暂时的做一个局部变量，等调用它的node释放了，它就会被释放
//addlinedata的时候，传入的linedata都已经在util中被处理好了，就是不带“，”的一组数据，包括了一个实体的所有属性。
//使用的时候，由于属性是按顺序存放的，因此直接调用该列的数据即可。
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
