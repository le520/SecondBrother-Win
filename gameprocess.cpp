#include "gameprocess.h"

GameProcess::GameProcess(QSettings *initIniFile,Ui::MainWindow *initUi)
{
    iniFile=initIniFile;
    codec = QTextCodec::codecForName("GBK");
    ui=initUi;
    isStop=false;
}
GameProcess::~GameProcess()
{
    delete m_manager;
}
void GameProcess::run(){
    m_manager = new QNetworkAccessManager();
    isStop=false;
    init();
    while(!isStop){
        getBaseData();
        for(int i=0;i<gameState.round&&!isStop;i++){
             fight();
        }
        repairEquipment();
    }
}
void GameProcess::stop(){
    isStop=true;
}
void GameProcess::restart(){
    gameState=GameState();
}
void GameProcess::recovery(){
    GetResult data=get(gameState.url);
    if(data.code==302){
        errorProcess(data);
        data=get(gameState.url);
        if(data.code==302){
            return;
        }
    }
    if(data.result.contains(codec->toUnicode("���ڵ�ͼ"))){
        QRegExp rx(codec->toUnicode("<a href=\"(.*)\">���ڵ�ͼ"));
        rx.lastIndexIn(data.result);
        data=get(gameState.baseUrl+rx.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
        if(data.result.contains(codec->toUnicode("����"))){
            QRegExp rx2(codec->toUnicode("<a href=\"(.*)\">����"));
            rx2.lastIndexIn(data.result);
            data=get(gameState.baseUrl+rx2.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
            QRegExp rx3(codec->toUnicode("NpcInfo(.*)'>"));
            data.result=data.result.mid(data.result.lastIndexOf(codec->toUnicode("Map/NpcInfo")));
            data.result=data.result.left(data.result.indexOf("</a>"));
            rx3.indexIn(data.result);
            data=get(gameState.baseUrl+"/Business/Return/ReturnBlood"+rx3.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
            msleep(gameState.interval);
            const QUrl url = QUrl::fromUserInput(gameState.baseUrl+"/Business/Return/ReturnBloodOk?sid="+gameState.sid);
            QString result;
            result=data.result;
            result=result.mid(result.indexOf(codec->toUnicode("<input name=\"npc\"")));
            result=result.left(result.indexOf(codec->toUnicode(">")));
            QRegExp rx4(codec->toUnicode("value=\"(.*)\""));
            rx4.indexIn(result);
            result=data.result;
            result=result.mid(result.indexOf(codec->toUnicode("<input name=\"token\"")));
            result=result.left(result.indexOf(codec->toUnicode(">")));
            QRegExp rx5(codec->toUnicode("value=\"(.*)\""));
            rx5.indexIn(result);
            QByteArray post_data;
            post_data.append("npc="+rx4.cap(1)+"&");
            post_data.append("token="+rx5.cap(1));
            QNetworkRequest qnr(url);
            QNetworkReply* reply = m_manager->post(qnr,post_data);
            QEventLoop eventLoop;
            connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
            reply->deleteLater();
            reply = nullptr;
        }
    }
}
void GameProcess::getBaseData(){
    GetResult data=get(gameState.url);
    if(data.code==302){
        errorProcess(data);
        data=get(gameState.url);
        if(data.code==302&&data.result.contains(codec->toUnicode("MonsterFight"))){
            data=get(gameState.baseUrl+data.result);
        }else if(data.code==302){
            return;
        }
    }
    if(data.result.contains(codec->toUnicode("״̬"))){
        QRegExp rx(codec->toUnicode("<a href=\"(.*)\">״̬"));
        rx.lastIndexIn(data.result);
        data=get(gameState.baseUrl+rx.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
        if(data.result.contains(codec->toUnicode("�ǳ�"))){
            QString result=data.result;
            result=result.mid(result.lastIndexOf(codec->toUnicode("�ǳ�")));
            result=result.left(result.indexOf(codec->toUnicode("/a>")));
            QRegExp rx2(codec->toUnicode(">(.*)<"));
            rx2.indexIn(result);
            ui->nameLabel->setText(rx2.cap(1));
            result=data.result;
            result=result.mid(result.lastIndexOf(codec->toUnicode("�ȼ�")));
            result=result.left(result.indexOf(codec->toUnicode("/>")));
            QRegExp rx3(codec->toUnicode(":(.*)<"));
            rx3.indexIn(result);
            ui->levelLabel->setText(rx3.cap(1));
            gameState.level=rx3.cap(1).toInt();
        }
    }
}
void GameProcess::repairEquipment(){
    GetResult data=get(gameState.url);
    if(data.code==302){
        errorProcess(data);
        data=get(gameState.url);
        if(data.code==302){
            return;
        }
    }
    QRegExp rx(codec->toUnicode("<a href=\"(.*)\">���ڵ�ͼ"));
    rx.lastIndexIn(data.result);
    data=get(gameState.baseUrl+rx.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
    QRegExp rx2(codec->toUnicode("<a href=\"(.*)\">������"));
    rx2.lastIndexIn(data.result);
    data=get(gameState.baseUrl+rx2.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
    QRegExp rx3(codec->toUnicode("NpcInfo(.*)'>"));
    data.result=data.result.mid(data.result.lastIndexOf(codec->toUnicode("Map/NpcInfo")));
    data.result=data.result.left(data.result.indexOf("</a>"));
    rx3.indexIn(data.result);
    data=get(gameState.baseUrl+"/Business/Return/ReturnDurabilityOk"+rx3.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
}
void GameProcess::fight(){
    GetResult data=get(gameState.url);
    bool isRecovey=false;
    if(data.code==302){
        errorProcess(data);
        data=get(gameState.url);
    }
    if(data.code==302&&data.result.contains(codec->toUnicode("MonsterFight"))){
        data=get(gameState.baseUrl+data.result);
    }else if(data.result.contains(codec->toUnicode("����"))){
    }else{
        QRegExp rx(codec->toUnicode("<a href=\"(.*)\">"+gameState.monsterName.toLocal8Bit()));
        rx.lastIndexIn(data.result);
        data=get(gameState.baseUrl+rx.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
        data=get(gameState.baseUrl+data.result);
    }
    while (data.result.contains(codec->toUnicode("����"))) {
        QRegExp rx(codec->toUnicode("<a id=\"atk\" href=\"(.*)\">����"));
        rx.lastIndexIn(data.result);
        data=get(gameState.baseUrl+rx.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
    }
    if(data.result.contains(codec->toUnicode("սʤ��"))){
        gameState.killed++;
        ui->killedNumLabel->setText(QString::number(gameState.killed));
        QString result=data.result;
        result=result.mid(result.lastIndexOf(codec->toUnicode("���� +"))+4);
        gameState.ex+=result.left(result.indexOf("<")).toInt();
        ui->exLabel->setNum(gameState.ex);
        result=data.result;
        result=result.mid(result.lastIndexOf(codec->toUnicode("��Ǯ +"))+4);
        gameState.money+=result.left(result.indexOf("(")).toInt();
        ui->moneyLabel->setNum(gameState.money);
        if(gameState.recovery){
            int left,all;
            QString result=data.result;
            result=result.mid(result.lastIndexOf(codec->toUnicode("������:"))+4);
            left=result.left(result.indexOf("/")).toInt();
            result=data.result;
            result=result.mid(result.lastIndexOf(codec->toUnicode("������:")));
            result=result.mid(result.indexOf(codec->toUnicode("/"))+1);
            all=result.left(result.indexOf("<")).toInt();
            if(((double)left/all)<0.5){
                isRecovey=true;
            }
        }
        if(gameState.pickup){
            for(int i=0;i<4;i++){
                if(!gameState.itemName[i].isEmpty())
                {
                    if(gameState.itemNum[i] > gameState.itemPicked[i])
                    {
                        if(data.result.contains(codec->toUnicode(gameState.itemName[i].toLocal8Bit())))
                        {
                            QRegExp rx("<a href=\"(.*)\">" + gameState.itemName[i]);
                            rx.lastIndexIn(data.result);
                            data=get(gameState.baseUrl+rx.cap(1).remove(QString("amp;"),Qt::CaseInsensitive));
                            gameState.itemPicked[i]++;
                            if(i==0){
                                ui->item1PickedLabel->setNum(gameState.itemPicked[i]);
                            }else if(i==1){
                                ui->item2PickedLabel->setNum(gameState.itemPicked[i]);
                            }else if(i==2){
                                ui->item3PickedLabel->setNum(gameState.itemPicked[i]);
                            }else if(i==3){
                                ui->item4PickedLabel->setNum(gameState.itemPicked[i]);
                            }
                        }
                    }
                }
            }
        }
        if(isRecovey){
            recovery();
        }
    }
}
void GameProcess::errorProcess(const GetResult &getResult){
    GetResult result=get(gameState.baseUrl+getResult.result);
    if(result.code==302){
        errorProcess(result);
    }else if(result.result.contains(codec->toUnicode("������Ϸ"))){
        QRegExp rx(codec->toUnicode("<a href=\"(.*)\">������Ϸ"));
        rx.indexIn(result.result);
        result = get(gameState.baseUrl+rx.cap(1));
    }else if(result.result.contains(codec->toUnicode("����������Ϸ"))){
        result.result=result.result.mid(result.result.indexOf(codec->toUnicode("���߽���")));
        QRegExp rx(codec->toUnicode("<a href=\"(.*)\">.����������Ϸ"));
        rx.indexIn(result.result);
        result = get(gameState.baseUrl+rx.cap(1));
    }else if(result.result.contains(codec->toUnicode("��������"))){
        QRegExp rx(codec->toUnicode("<a href=\"(.*)\">��������"));
        rx.indexIn(result.result);
        result = get(gameState.baseUrl+rx.cap(1));
    }
}
GetResult GameProcess::get(const QString &strUrl)
{
    msleep(gameState.interval);
    const QUrl url = QUrl::fromUserInput(strUrl);

    QNetworkRequest qnr(url);
    QNetworkReply* reply = m_manager->get(qnr);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    GetResult replyData;
    replyData.result = reply->readAll();
    replyData.code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString redirectAttr = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    if (reply->error()
        || 302 == replyData.code //״̬��300 Multiple Choices���Ȳ��Ǵ���Ҳ�����ض���Ӧ����qt bug
        || !redirectAttr.isNull())
    {
         replyData.result=redirectAttr;
    }
    reply->deleteLater();
    reply = nullptr;
    return replyData;
}
void GameProcess::init(){
    gameState.baseUrl=iniFile->value("Base/baseUrl").toString();
    gameState.url=iniFile->value("Base/url").toString();
    gameState.lastUrl=iniFile->value("Base/url").toString();
    gameState.sid=gameState.url.mid(gameState.url.lastIndexOf("sid=")+4);
    gameState.pickup=iniFile->value("Base/pickup").toInt();
    gameState.recovery=iniFile->value("Base/recovery").toInt();
    gameState.interval=iniFile->value("Base/interval").toInt();
    gameState.round=iniFile->value("Base/round").toInt();
    gameState.monsterName=iniFile->value("Base/MonsterName").toString();
    gameState.itemNum[0]=iniFile->value("Pickup/item1Num").toInt();
    gameState.itemName[0]=iniFile->value("Pickup/item1Name").toString();
    gameState.itemNum[1]=iniFile->value("Pickup/item2Num").toInt();
    gameState.itemName[1]=iniFile->value("Pickup/item2Name").toString();
    gameState.itemNum[2]=iniFile->value("Pickup/item3Num").toInt();
    gameState.itemName[2]=iniFile->value("Pickup/item3Name").toString();
    gameState.itemNum[3]=iniFile->value("Pickup/item4Num").toInt();
    gameState.itemName[3]=iniFile->value("Pickup/item4Name").toString();
}
