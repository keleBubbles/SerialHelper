#ifndef SERIALHELPER_H
#define SERIALHELPER_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>

class SerialHelper:public QMainWindow
{
    Q_OBJECT
public:
    SerialHelper(QWidget *parent = nullptr);
    ~SerialHelper();

    void ReceiveAeraInit(void);//接收区的初始化
    void SendAreaInit(void);//发送区的初始化
    void SetupInit(void);//右边菜单
    void BeginUSART(void);
    void USART(void);//处理连接，点击开始连接按钮后，触发的函数
    void timerEvent(QTimerEvent* e) override;
private:
    QPlainTextEdit *sendAera;//发送区
    QPlainTextEdit *receiveAera;//接收区
    QPushButton *sendButton; //发送按钮
    QPushButton *startUSART; //开始检测
    QPushButton *endUSART; //停止检测

    QComboBox * portNumber; //串口号
    QComboBox * baudRate;//波特率
    QComboBox * dataSize;//数据位
    QComboBox * stopSize;//停止位
    QComboBox * check;//校验位
    QComboBox * receiveMode;//接收格式
    QComboBox * sendMode;//发送格式

    QSerialPort *serialPort;
    QVector<QString> ports; //串口号列表
    int refreshTimerId;//定时器编号

};

#endif // SERIALHELPER_H
