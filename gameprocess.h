#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <qdebug.h>
#include <QSettings>
#include <QRegExp>
#include <QTextCodec>
#include "ui_mainwindow.h"
#include <QElapsedTimer>
#include <QThread>
QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
QT_END_NAMESPACE
struct GameState{
    QString baseUrl;
    QString lastUrl;
    QString url;
    QString name;
    QString monsterName;
    QString state;
    QString sid;
    QString itemName[4];
    double avgBlood=0;
    int itemNum[4];
    int itemPicked[4]{0,0,0,0};
    int pickup = 0;
    int recovery = 0;
    int level;
    int killed = 0;
    int interval = 0;
    int round;
    int ex = 0;
    int money = 0;
};
struct GetResult{
    QString result;
    int code;
};
class GameProcess:public QThread
{
public:
    GameProcess(QSettings *initIniFile,Ui::MainWindow *initUi);
    ~GameProcess();
    void stop();
    void restart();
protected:
    void run();
private:
    void fight();
    QString chooseMonster(GetResult &data);
    QString killMonster(GetResult &data);
    void recovery();
    void getBaseData();
    void repairEquipment();
    void init();
    void errorProcess(const GetResult &getResult);
    GetResult get(const QString &strUrl);
private:
    QNetworkAccessManager *m_manager;
    QSettings *iniFile;
    GameState gameState;
    QTextCodec *codec;
    Ui::MainWindow *ui;
    bool isStop;
};

#endif // GAMEPROCESS_H
