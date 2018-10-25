#include "tcpserver.h"
#include "threadhandle.h"

TcpServer::TcpServer(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
     tcpClient = new  QHash<int,TcpSocket *>;
     setMaxPendingConnections(numConnections);

}

TcpServer::~TcpServer()
{
    emit this->sentDisConnect(-1);
    delete tcpClient;
}

void TcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    if (tcpClient->size() > maxPendingConnections())//继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    //新来的连接加入到新的线程
    auto th = ThreadHandle::getClass().getThread();//作用？？？？？？？？？？？
    auto tcpTemp = new TcpSocket(socketDescriptor);//创建一个新的连接
//    QThread *th = ThreadHandle::getClass().getThread();
//    TcpSocket *tcpTemp = new TcpSocket(socketDescriptor);

    QString ip =  tcpTemp->peerAddress().toString();//获得新连接进来的ip和端口号
    qint16 port = tcpTemp->peerPort();

    connect(tcpTemp,&TcpSocket::sockDisConnect,this,&TcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this,&TcpServer::sentDisConnect,tcpTemp,&TcpSocket::disConTcp);//断开信号

    connect(tcpTemp,&TcpSocket::showMesstoServer,this,&TcpServer::sendtomain);//~~~~~~~~~


    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中，把这个新的socket描述符和连接信息加到，socket链表当中去

    emit connectClient(socketDescriptor,ip,port);//???作用？
    emit flushmainshow(tcpClient);
}

void TcpServer::sendtomain( QString remark, QString flag)
{
    //qDebug()<<"in tcpserver123";
    //qDebug()<<"current thread in tcpserver"<<QThread::currentThreadId();
    emit tomain(remark,flag);
}

void TcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    emit sockDisConnect(handle,ip,prot);
    emit flushmainshow(tcpClient);
}


void TcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
    emit flushmainshow(tcpClient);
}
