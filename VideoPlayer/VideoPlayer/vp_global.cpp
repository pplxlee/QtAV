#include "vp_global.h"
#include <QTranslator>
#include <QFile>
#include <QSettings>

VPConfigure* VPConfigure::s_configure = new VPConfigure;

QString trloadEn()
{
    g_appTranslator->load(":/tr/tr_en.qm");
    return QString("en");
}

QString trloadZhCh()
{
    g_appTranslator->load(":/tr/tr_zh_CN.qm");
    return QString("zh_CN");
}

QString trload(const QString &lg)
{
    if(QFile(":/tr/tr_" + lg + ".qm").exists()) {
        g_appTranslator->load(":/tr/tr_" + lg + ".qm");
        return QString(lg);
    }
    else {
        trloadZhCh(); // 否则默认使用中文
        return QString("zh_CN");
    }
}


void VPConfigure::initSettings(const QString &organizationName, const QString &appName)
{
    mSettings = new QSettings(organizationName, appName);
}
