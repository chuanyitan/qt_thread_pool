#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include <QTimer>


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();
    QByteArray handleData(QByteArray data,const QString & ip, qint16 port);//用来处理数据的函数

signals:
    //void readData(const int,const QString &,const quint16,const QByteArray &);
    void showMesstoServer(QString remark,QString flag);
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);

protected slots:
    void readData();//接收数据
    void startNext();//处理下一个

    void savecmd(QString ip,QString cmd);//接收命令
    void sendcmd();//定时发送命令

    void linkstatus(int);//客户端状态，0，表示离线,1表示在线

protected:
    QFutureWatcher<QByteArray> watcher;
    QQueue<QByteArray> datas;
private:
    qintptr socketID;
    QMetaObject::Connection dis;
    QTimer timer;
};

#endif // TCPSOCKET_H
