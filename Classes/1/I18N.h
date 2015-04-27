/*
    �ļ�����    I18N.h
    �衡����    ���ʻ�����
    �����ˣ�    ��ľͷ (���ͣ�http://www.benmutou.com)
*/

#ifndef __I18N_H__
#define __I18N_H__

#include "cocos2d.h"
#include "EnumStrKey.h"

using namespace cocos2d;

class I18N : public Ref {
public:
    /* ��ȡ���� */
    static I18N* getInstance();
    
    /* ��ʼ�� */
    virtual bool init();

    /* ����Keyֵ��ȡ�ַ���,����const char*���� */
    const char* getcString(EnumStrKey enStrKey);

    /* �������͵�Keyֵ��ȡ�ַ���,����const char*���� */
    const char* getcStringByKey(int iKey);

private:
    /* �������ļ��ж�ȡ�ַ��������浽�ֵ��� */
    void loadStringsFromConf(const char* filePath);

    /* I18N���� */
    static I18N* m_I18N;

    /* ��Ϸ���õ����ַ����ֵ� */
    std::map<int, std::string> mStringMap;
};

#endif