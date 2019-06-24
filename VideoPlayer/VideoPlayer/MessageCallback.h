#ifndef MESSAGECALLBACK_H
#define MESSAGECALLBACK_H

#include <QFile>
#include <QDateTime>
#include <QMutex>

static QFile s_logFile;
static QMutex s_logMutex;

void openLogFile(QString file)
{
    QString dir_str = QString("./log/") + QDateTime::currentDateTime().toString("yyyyMMdd");

    // 检查目录是否存在，若不存在则新建


    QString filePathName = dir_str + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".log";
    s_logFile.setFileName(filePathName);
    s_logFile.open(QIODevice::WriteOnly);
}

void closeLogFile()
{
    s_logFile.close();
    // 若无日志产生，则删除日志文件
    if(s_logFile.size() <= 0)
    {
        s_logFile.remove();
    }
}

void appendLogFile(const char *msg)
{
    s_logFile.write(msg);
    s_logFile.write("\r\n");
    s_logFile.flush();
}

void printToConsole(const char *msg)
{
#ifdef QT_DEBUG
    std::cout << msg << std::endl;
#else
    Q_UNUSED(msg);
#endif// QT_DEBUG
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    QString cntDateTimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
    QByteArray localMsg = msg.toLocal8Bit();
//    QByteArray localMsg = msg.toUtf8();

    s_logMutex.lock();
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
    s_logMutex.unlock();
}

#endif // MESSAGECALLBACK_H
