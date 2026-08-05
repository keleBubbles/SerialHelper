#include "serialhelper.h"


void SerialHelper::ReceiveAeraInit(void){
    receiveAera = new QPlainTextEdit(this);
    receiveAera->setFixedSize(800,400);
    receiveAera->move(30,20);
    receiveAera->setReadOnly(true);

    QPushButton *clearReceive = new QPushButton(this);
    clearReceive->setText(tr("清空接收区"));
    clearReceive->setFixedSize(150,50);
    clearReceive->move(680,430);
    connect(clearReceive,&QPushButton::clicked,receiveAera,&QPlainTextEdit::clear);
}
void SerialHelper:: SendAreaInit(void){
    sendAera = new QPlainTextEdit(this);
    sendAera->setFixedSize(800,100);
    sendAera->move(30,500);
    //发送按钮
    sendButton = new QPushButton(tr("发送"),this);
    sendButton->setFixedSize(150,50);
    sendButton->move(500,610);
    sendButton->setDisabled(true);
    connect(sendButton,&QPushButton::clicked,this,[this](){
        //发送数据
        QString data = sendAera->toPlainText();//从接收区拿数据
        qDebug() << data;
        if(sendMode->currentText() == "HEX"){
            QByteArray arr;
            for(int i = 0;i<data.size();++i){
                if(data[i] == ' ') continue;
                int num = data.mid(i,2).toUInt(nullptr,16);
                ++i;
                arr.append(num);
            }
            serialPort->write(arr);
        }else {
            serialPort->write(data.toUtf8());
        };

    });

    QPushButton *clearSend = new QPushButton(this);
    clearSend->setText(tr("清空发送区"));
    clearSend->setFixedSize(150,50);
    clearSend->move(680,610);
    connect(clearSend,&QPushButton::clicked,sendAera,&QPlainTextEdit::clear);


}

void SerialHelper::SetupInit(void){
    this->portNumber = new QComboBox(this);

    this->baudRate = new QComboBox(this);
    this->dataSize = new QComboBox(this);
    this->stopSize = new QComboBox(this);
    this->check = new QComboBox(this);
    this->receiveMode = new QComboBox(this);
    this->sendMode = new QComboBox(this);

    this->baudRate->addItem("4800");
    this->baudRate->addItem("9600");
    this->baudRate->addItem("19200");

    this->dataSize->addItem("8");

    this->stopSize->addItem("1");
    this->stopSize->addItem("1.5");
    this->stopSize->addItem("2");

    this->check->addItem(tr("无"));
    this->check->addItem(tr("奇校验"));
    this->check->addItem(tr("偶校验"));

    this->receiveMode->addItem("HEX");
    this->receiveMode->addItem("文本");

    this->sendMode->addItem("HEX");
    this->sendMode->addItem("文本");

    QLabel *portLabel = new QLabel(tr("串口号"),this);
    QLabel *baudLabel = new QLabel(tr("波特率"),this);
    QLabel *dataLabel = new QLabel(tr("数据位"),this);
    QLabel *stopLabel = new QLabel(tr("停止位"),this);
    QLabel *checkLabel = new QLabel(tr("校验位"),this);
    QLabel *receiveLabel = new QLabel(tr("接收格式"),this);
    QLabel *sendLabel = new QLabel(tr("发送格式"),this);

    QVector<QComboBox*>setups;
    setups.push_back(portNumber);
    setups.push_back(baudRate);
    setups.push_back(dataSize);
    setups.push_back(stopSize);
    setups.push_back(check);
    setups.push_back(receiveMode);
    setups.push_back(sendMode);

    QVector<QLabel*> labels;
    labels.push_back(portLabel);
    labels.push_back(baudLabel);
    labels.push_back(dataLabel);
    labels.push_back(stopLabel);
    labels.push_back(checkLabel);
    labels.push_back(receiveLabel);
    labels.push_back(sendLabel);

    for(int i = 0; i <setups.size();++i){
        setups[i]->setFixedSize(200,50);
        setups[i]->move(850,20 + i * 80);
        labels[i]->move(1080,25+i*80);
    }
}

void SerialHelper::BeginUSART()
{
    startUSART = new QPushButton(tr("串口连接"),this);
    endUSART = new QPushButton(tr("串口断开"),this);
    startUSART->setFixedSize(150,50);
    endUSART->setFixedSize(150,50);
    startUSART->move(850,600);
    endUSART->move(1000,600);

    endUSART->setDisabled(true);

    connect(endUSART,&QPushButton::clicked,this,[this](){
        sendButton->setDisabled(true);
        startUSART->setDisabled(false);
        endUSART->setDisabled(true);
        //断开连接
        serialPort->close();
    });
    connect(startUSART,&QPushButton::clicked,this,[this](){
        //连接
        if(portNumber->currentText()!="") {
            startUSART->setDisabled(true);
            endUSART->setDisabled(false);
            sendButton->setDisabled(false);
            USART();
        }else {
            QMessageBox::critical(this,tr("串口打开失败"),tr("请确认串口是否连接正常"));
        }
    });
}

void SerialHelper::USART(void)//连接
{
    QSerialPort::BaudRate Baud;
    QSerialPort::DataBits Data;
    QSerialPort::StopBits Stop;
    QSerialPort::Parity Check;

    QString port = portNumber->currentText();
    QString baud = baudRate->currentText();
    QString data = dataSize->currentText();
    QString stop = stopSize->currentText();
    QString ch = check->currentText();

    if (baud == "4800")
        Baud = QSerialPort::Baud4800;
    else if (baud == "9600")
        Baud = QSerialPort::Baud9600;
    else if (baud == "19200")
        Baud = QSerialPort::Baud19200;

    if (data == "8")
        Data = QSerialPort::Data8;

    if (stop == "1")
        Stop = QSerialPort::OneStop;
    else if (stop == "1.5")
        Stop = QSerialPort::OneAndHalfStop;
    else if (stop == "2")
        Stop = QSerialPort::TwoStop;

    if (ch == QString::fromLocal8Bit("无"))
        Check = QSerialPort::NoParity;
    else if (ch == QString::fromLocal8Bit("奇校验"))
        Check = QSerialPort::OddParity;
    else if (ch == QString::fromLocal8Bit("偶校验"))
        Check = QSerialPort::EvenParity;

    serialPort = new QSerialPort(this);
    serialPort->setBaudRate(Baud);
    serialPort->setDataBits(Data);
    serialPort->setParity(Check);
    serialPort->setStopBits(Stop);
    serialPort->setPortName(port);

    if(serialPort->open(QSerialPort::ReadWrite)){
        //打开成功
        connect(serialPort,&QSerialPort::readyRead,this,[this](){
            auto data = serialPort->readAll();
            if(receiveMode->currentText() == "HEX"){
                QString hex = data.toHex(' ');
                this->receiveAera->appendPlainText(hex);
            }else{
                QString str = QString(data);
                receiveAera->appendPlainText(str);
            }
        });
    }else{
        QMessageBox::critical(this,tr("串口打开失败"),tr("请确认串口是否连接正常"));
    }
}

//固定写法
void SerialHelper::timerEvent(QTimerEvent* e){
    if((e->timerId() != refreshTimerId))
        return;

    QVector<QString> temp;
    for(const QSerialPortInfo& info : QSerialPortInfo::availablePorts()){
        temp.push_back(info.portName());
    }
    std::sort(temp.begin(),temp.end());
    if(temp != this->ports){
        this->portNumber->clear();
        this->ports = temp;
        for(auto & a :ports) portNumber->addItem(a);
    }
}

SerialHelper::SerialHelper(QWidget *parent):
    QMainWindow(parent)
{
    this->setFixedSize(1200,750);
    this->setWindowTitle(tr("串口助手"));
    ReceiveAeraInit();//调用接收区初始化函数
    SendAreaInit();
    SetupInit();
    BeginUSART();
    refreshTimerId = this->startTimer(1000);//定时器


}

SerialHelper::~SerialHelper(){

}
