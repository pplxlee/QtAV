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
    void translate();

private slots:
    void translateEn();
    void translateZhCh();

private:
    Ui::MainWindow *ui;

    QAction *actionQuitFullscreen;
    QAction *actionStopTrack;
};

#endif // MAINWINDOW_H
