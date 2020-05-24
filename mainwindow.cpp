#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFileInfo fileInfo("sysConfig.ini");
    if(!fileInfo.isFile())
    {
        writeConfig();
    }else{
        iniFile = new QSettings("sysConfig.ini", QSettings::IniFormat);
    }
    uiInit();
    game=new GameProcess(iniFile,ui);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete iniFile;
    delete game;
}
void MainWindow::uiInit(){
    ui->killedNumLabel->setNum(0);
    ui->item1PickedLabel->setNum(0);
    ui->item2PickedLabel->setNum(0);
    ui->item3PickedLabel->setNum(0);
    ui->item4PickedLabel->setNum(0);
    ui->levelLabel->setNum(0);
    ui->exLabel->setNum(0);
    ui->moneyLabel->setNum(0);
    ui->nameLabel->setText("");
    ui->pauseButton->setText(QString::fromLocal8Bit("暂停"));
    ui->urlEdit->setText(iniFile->value("Base/url").toString());
    ui->roundEdit->setText(iniFile->value("Base/round").toString());
    ui->intervalEdit->setText(iniFile->value("Base/interval").toString());
    ui->monsterNameEdit->setText(iniFile->value("Base/monsterName").toString());
    if(iniFile->value("Base/pickup").toInt()){
        ui->pickupCheckBox->setCheckState(Qt::Checked);
    }
    if(iniFile->value("Base/recovery").toInt()){
        ui->recoveryCheckBox->setCheckState(Qt::Checked);
    }
    ui->item4NameEdit->setText(iniFile->value("Pickup/item4Name").toString());
    ui->item4NumEdit->setText(iniFile->value("Pickup/item4Num").toString());
    ui->item3NameEdit->setText(iniFile->value("Pickup/item3Name").toString());
    ui->item3NumEdit->setText(iniFile->value("Pickup/item3Num").toString());
    ui->item2NameEdit->setText(iniFile->value("Pickup/item2Name").toString());
    ui->item2NumEdit->setText(iniFile->value("Pickup/item2Num").toString());
    ui->item1NameEdit->setText(iniFile->value("Pickup/item1Name").toString());
    ui->item1NumEdit->setText(iniFile->value("Pickup/item1Num").toString());
}
void MainWindow::writeConfig()
{

    //创建配置文件
    iniFile = new QSettings("sysConfig.ini", QSettings::IniFormat);
    //组开始
    iniFile->beginGroup(tr("Base"));
    //设置键值对
    iniFile->setValue(tr("url"), tr(""));
    iniFile->setValue(tr("baseUrl"), tr("http://zh.8gwap.cn"));
    iniFile->setValue(tr("monsterName"), tr(""));
    iniFile->setValue(tr("round"), tr("100"));
    iniFile->setValue(tr("interval"), tr("0"));
    iniFile->setValue(tr("pickup"),tr("0"));
    iniFile->setValue(tr("recovery"),tr("2"));
    //组结束
    iniFile->endGroup();
    //组开始
    iniFile->beginGroup(tr("Pickup"));
    //设置键值对
    iniFile->setValue(tr("item1Name"), tr(""));
    iniFile->setValue(tr("item1Num"), tr(""));
    iniFile->setValue(tr("item2Name"), tr(""));
    iniFile->setValue(tr("item2Num"), tr(""));
    iniFile->setValue(tr("item3Name"), tr(""));
    iniFile->setValue(tr("item3Num"), tr(""));
    iniFile->setValue(tr("item4Name"), tr(""));
    iniFile->setValue(tr("item4Num"), tr(""));
    //组结束
    iniFile->endGroup();
}

void MainWindow::readConfig()
{
    //获取配置文件名
    QString fileName=iniFile->fileName();
    qDebug().noquote()<<"fileName:"<<fileName;

    //判断键是否存在
    //bool key=iniFile->contains("Helen/age");
    //读取键值对
    int round = iniFile->value("Base/round").toInt();
    qDebug()<<round;

}

void MainWindow::clearConfig()
{
    iniFile->clear();
}
void MainWindow::saveConfig(){
    iniFile->setValue(tr("Base/url"), ui->urlEdit->text());
    iniFile->setValue(tr("Base/monsterName"), ui->monsterNameEdit->text());
    iniFile->setValue(tr("Base/round"), ui->roundEdit->text());
    iniFile->setValue(tr("Base/interval"),ui->intervalEdit->text());
    iniFile->setValue(tr("Base/pickup"),ui->pickupCheckBox->checkState());
    iniFile->setValue(tr("Base/recovery"),ui->recoveryCheckBox->checkState());
    iniFile->setValue(tr("Pickup/item1Name"), ui->item1NameEdit->text());
    iniFile->setValue(tr("Pickup/item1Num"), ui->item1NumEdit->text());
    iniFile->setValue(tr("Pickup/item2Name"), ui->item2NameEdit->text());
    iniFile->setValue(tr("Pickup/item2Num"), ui->item2NumEdit->text());
    iniFile->setValue(tr("Pickup/item3Name"), ui->item3NameEdit->text());
    iniFile->setValue(tr("Pickup/item3Num"), ui->item3NumEdit->text());
    iniFile->setValue(tr("Pickup/item4Name"), ui->item4NameEdit->text());
    iniFile->setValue(tr("Pickup/item4Num"), ui->item4NumEdit->text());
}
void MainWindow::on_startButton_clicked()
{
    saveConfig();
    if(ui->startButton->text()==QString::fromLocal8Bit("开始")){
        uiInit();
        ui->startButton->setText(QString::fromLocal8Bit("停止"));
        game->restart();
        game->start();
    }else{
        ui->startButton->setText(QString::fromLocal8Bit("开始"));
        game->stop();
    }
}

void MainWindow::on_pauseButton_clicked()
{
    saveConfig();
    if(ui->startButton->text()==QString::fromLocal8Bit("停止")){
        if(ui->pauseButton->text()==QString::fromLocal8Bit("暂停")){
            ui->pauseButton->setText(QString::fromLocal8Bit("继续"));
            game->stop();
        }else{
            ui->pauseButton->setText(QString::fromLocal8Bit("暂停"));
            game->start();
        }
    }
}
void MainWindow::on_recoveryCheckBox_stateChanged(int arg1)
{
    iniFile->setValue("Base/recovery",arg1);
}

void MainWindow::on_pickupCheckBox_stateChanged(int arg1)
{
    iniFile->setValue("Base/pickup",arg1);
}
