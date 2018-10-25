#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include<QDebug>
#include<QDateTime>


TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);//读数据的信号与曹
    dis = connect(this,&TcpSocket::disconnected,//断开连接的信号与曹
        [&](){
            qDebug() << "disconnect" ;
            linkstatus(0);
            emit sockDisConnect(socketID,this->peerAddress().toString().remove("::ffff:"),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
  //  connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);//???watcher的作用
  //  connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;
    linkstatus(1);//???
    connect(&timer,SIGNAL(timeout()),this,SLOT(sendcmd()));
    timer.setParent(this);
    timer.start(1000);
}

TcpSocket::~TcpSocket()
{
    linkstatus(0);
}

void TcpSocket::linkstatus(int flag)//查询是否在线
{

}

void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void TcpSocket::disConTcp(int i)
{
    qDebug()<<"client is disconnect!";
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::readData()
{
    auto data  = this->readAll();
    QString ip=this->peerAddress().toString().remove("::ffff:");
    QByteArray temp=data;

    qDebug()<<"read data:"<<data<<" "<<this->peerAddress().toString().remove("::ffff:");
//    savecmd(ip,strHex);
//    if (!watcher.isRunning())//放到异步线程中处理。
//    {
//        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
//    }
}



QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
    QTime time;
    time.start();
    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    qDebug()<<"next:";

    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString().remove("::ffff:"),this->peerPort()));
    }
}

void TcpSocket::savecmd(QString ip,QString cmd)
{


}

void TcpSocket::sendcmd()//定时查询是否有命令发送
{
     QString ip=this->peerAddress().toString().remove("::ffff:");

}


