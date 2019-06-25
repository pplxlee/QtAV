#include "MainWindow.h"
#include "vp_global.h"
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QMutex>
#include <QStyleFactory>
#include <QTranslator>
#include <QLibraryInfo>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#include <WinBase.h>
#endif

#define PRINT_CONSOLE

static QFile sLogFile;
static QMutex sLogMutex;

void printToConsole(const char *msg)
{
#ifdef PRINT_CONSOLE
    std::cout << msg << std::endl;
#else
    Q_UNUSED(msg);
#endif// QT_DEBUG
}

void appendLogFile(const char *msg)
{
    sLogFile.write(msg);
    sLogFile.write("\r\n");
    sLogFile.flush();
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    QString cntDateTimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
    QByteArray localMsg = msg.toLocal8Bit();

    sLogMutex.lock();
    switch (type) {
    case QtDebugMsg:    // 仅仅在控制台显示
        localMsg.insert(0, "[D] " + cntDateTimeStr);
        printToConsole(localMsg.constData());
        break;
    case QtInfoMsg:     // 需要打入日志
        localMsg.insert(0, "[I] " + cntDateTimeStr);
        printToConsole(localMsg.constData());
        appendLogFile(localMsg.constData());
        break;
    case QtWarningMsg:  // 需要打入日志
        localMsg.insert(0, "[W] " + cntDateTimeStr);
        printToConsole(localMsg.constData());
        appendLogFile(localMsg.constData());
        break;
    case QtCriticalMsg: // 需要打入日志
        localMsg.insert(0, "[C] " + cntDateTimeStr);
        printToConsole(localMsg.constData());
        appendLogFile(localMsg.constData());
        break;
    case QtFatalMsg:    // 需要打入日志
        localMsg.insert(0, "[F] " + cntDateTimeStr);
        printToConsole(localMsg.constData());
        appendLogFile(localMsg.constData());
        break;
    }
    sLogMutex.unlock();
}

bool installLogSystem()
{
    QString dir_str = QString("./log/") + QDateTime::currentDateTime().toString("yyyyMMdd");
    QDir dir;
    if (!dir.exists(dir_str) && !dir.mkpath(dir_str)) {
        qWarning() << "Make path failed!";
        return false;
    }
    QString filePathName = dir_str + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".log";
    sLogFile.setFileName(filePathName);
    if(!sLogFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Open file failed!";
        return false;
    }
    qInstallMessageHandler(myMessageOutput);
    return true;
}

void closeLogFile()
{
    sLogFile.close();
    // 若无日志产生，则删除日志文件
    if(sLogFile.size() <= 0) {
        sLogFile.remove();
    }
}

QTranslator *g_appTranslator;

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,10,0))
    // 不显示帮助按钮
    QCoreApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
#endif
    // 保证菜单行为与Win下的一致性
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    // 启动高Dpi缩放支持
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    // 设置程序路径为工作路径
    QDir::setCurrent(QApplication::applicationDirPath());

    // Log system
    if(!installLogSystem())
        qWarning() << "Install log systom failed!";
    qInfo()<<"Log system installed!";

    // 设置窗口风格
    qDebug()<<"main()"<<"Style keys"<<QStyleFactory::keys();
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // 设置应用程序显示名
    QGuiApplication::setApplicationDisplayName(QObject::tr("Video Player"));

    // TODO: 设置文件关联

//    QTranslator appTranslator;
    g_appTranslator = new QTranslator;
    trload(QLocale::system().name());
//    if(QFile(":/tr/tr_" + QLocale::system().name() + ".qm").exists()) {
//        g_appTranslator->load(":/tr/tr_" + QLocale::system().name() + ".qm");
//    }
//    else {
//        g_appTranslator->load(":/tr/tr_zh_CN.qm"); // 否则默认使用中文
//    }
    a.installTranslator(g_appTranslator);

    MainWindow w;
    w.show();

    // TODO: 读取并应用运行参数
    QString fileToPlay = QString();
    if(argc > 1)
    {
        QString argv1 = QString::fromLocal8Bit(argv[1]);
        if(QFileInfo(argv1).isFile())
        {
            qInfo()<<"main()"<<"Request play"<<argv1;
//            w.onPlayUrl(argv1);
        }
        else
        {
            QString languageType = argv1.section("-Language:", 1, 1).left(2);
            qInfo()<<"main()"<<"Request languageType is"<< languageType;
            QString qmfile;
            if(languageType.compare("en", Qt::CaseInsensitive) == 0) {
                qmfile = ":/tr/tr_en.qm";
            }
            else if (languageType.compare("zh", Qt::CaseInsensitive) == 0) {
                qmfile = ":/tr/tr_zh_CN.qm";
            }
            if(!qmfile.isEmpty()) {
                qDebug()<<"qmfile is " << qmfile;
                g_appTranslator->load(qmfile);
                a.installTranslator(g_appTranslator);
                w.translate();
            }
        }
    }
    else
    {
        qInfo()<<"main()"<<"No addtional args.";
    }

#ifdef _WIN32
    // 阻止系统自动熄屏或休眠
    ::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED
                              | ES_DISPLAY_REQUIRED);
#endif

    int res = QApplication::exec();

#ifdef _WIN32
    // 程序结束时，恢复系统自动熄屏或休眠
    ::SetThreadExecutionState(ES_CONTINUOUS);
#endif

    closeLogFile();     // 退出程序前关闭日志文件
    return res;
}
