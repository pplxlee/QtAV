#include "vp_global.h"
#include <QTranslator>
#include <QFile>

void trloadEn()
{
    g_appTranslator->load(":/tr/tr_en.qm");
}

void trloadZhCh()
{
    g_appTranslator->load(":/tr/tr_zh_CN.qm");
}

void trload(const QString &lg)
{
    if(QFile(":/tr/tr_" + lg + ".qm").exists()) {
        g_appTranslator->load(":/tr/tr_" + lg + ".qm");
    }
    else {
        trloadZhCh(); // 否则默认使用中文
    }
}
