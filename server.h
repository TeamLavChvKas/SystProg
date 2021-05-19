#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTextEdit>
#include <QWidget>
#include <Repository.h>

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(int nPort, QWidget *parent = nullptr);
    ~Server();

private:
    QTcpServer* tcpServer;
    QTextEdit*  txted;
    quint16     nextBlockSize;
    repository* repo;
    void sendToClient(QTcpSocket *pSocket, QByteArray arrBlock);

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
};
#endif // SERVER_H
