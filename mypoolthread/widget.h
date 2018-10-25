#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"tcpserver.h"
class  TcpServer;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    ~Widget();
public slots:
    void handel(QHash<int, TcpSocket *> *tcpClient);

private:
    Ui::Widget *ui;
    TcpServer *ser;
    int count = 0;
};

#endif // WIDGET_H
