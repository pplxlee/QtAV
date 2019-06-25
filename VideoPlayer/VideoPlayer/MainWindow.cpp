#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "vp_global.h"
#include <QAction>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    actionQuitFullscreen = new QAction(this);
    actionStopTrack = new QAction(this);

    // 快捷键设置
    // ------------
    for(QObject *menuchide : this->children())
    {
        // 设置所有的QAction快捷键为全局
        if(menuchide->metaObject()->className() == QString("QAction")){
            qobject_cast<QAction *>(menuchide)->setShortcutContext(Qt::ApplicationShortcut);
        }
    }
    ui->actionOpen_UDP->setShortcut(QString("Ctrl+U"));
    ui->actionOpen_File->setShortcut(QString("Ctrl+O"));
    ui->actionPlay->setShortcut(Qt::Key_Space);
    ui->actionSignal_Frame_Play->setShortcut(Qt::Key_S);
    ui->actionVertical_View->setShortcut(QString("Ctrl+D"));
    ui->actionPTZ_Position_Back->setShortcut(QString("Ctrl+H"));
    ui->actionEnter_Scout_Mode->setShortcut(QString("Ctrl+G"));
    ui->actionQuit_Scout_Mode->setShortcut(QString("Ctrl+Q"));
    actionQuitFullscreen->setShortcut(Qt::Key_Escape);
    actionStopTrack->setShortcut(QString("Shift+Esc"));


    // TODO: 连接各个信号槽
    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::translateEn);
    connect(ui->actionjian_ti_zhong_wen, &QAction::triggered, this, &MainWindow::translateZhCh);
    connect(this, &MainWindow::translated, this, &MainWindow::onTranslated);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::translateEn()
{
    mLanguage = trloadEn();
    translate();
}

void MainWindow::translateZhCh()
{
    mLanguage = trloadZhCh();
    translate();
}

void MainWindow::translate()
{
    ui->retranslateUi(this);
    emit translated();
}

void MainWindow::onTranslated()
{
    VPConfigure::getInstance()->settings()->setValue("language", mLanguage);
}
