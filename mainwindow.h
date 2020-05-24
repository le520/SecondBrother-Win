#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include "gameprocess.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void clearConfig();
    void writeConfig();
    void readConfig();
    void on_startButton_clicked();
    void uiInit();
    void saveConfig();

    void on_recoveryCheckBox_stateChanged(int arg1);

    void on_pickupCheckBox_stateChanged(int arg1);

    void on_pauseButton_clicked();

private:
    Ui::MainWindow *ui;
    QSettings *iniFile;
    GameProcess *game;
};
#endif // MAINWINDOW_H
