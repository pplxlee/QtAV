#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void translateEn();
    void translateZhCh();

private:
    void translate();
    void onTranslated();

signals:
    void translated();

private:
    Ui::MainWindow *ui;

    QAction *actionQuitFullscreen;
    QAction *actionStopTrack;

    QString mLanguage;
};

#endif // MAINWINDOW_H
