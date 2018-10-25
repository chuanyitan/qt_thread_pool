#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ser = new TcpServer;
    int port=6666;
    if(port==0)
    {
      port=6666;
    }
    ser->listen(QHostAddress::Any,port);
    connect(ser,&TcpServer::flushmainshow,this,&Widget::handel);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handel(QHash<int,TcpSocket *> *tcpClient)
{

    QHash<int,TcpSocket *>::iterator i;
    ui->listWidget->clear();

    for( i=tcpClient->begin(); i!=tcpClient->end(); ++i)
      {
        qDebug() << i.key() <<"        " << i.value()->peerAddress().toString().remove("::ffff:");
        ui->listWidget->addItem(i.value()->peerAddress().toString().remove("::ffff:"));
      }
    ui->num->setText(QString::number(tcpClient->size()));
}
