#include "server.h"
#include "QMessageBox"

#include <QLabel>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QTime>


Server::Server(int port, QWidget *parent)
    : QWidget(parent)
{
    repo = new repository();
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(0, "Server Error", "Unable to start the server:" + tcpServer->errorString());
        tcpServer->close();
        return;
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    txted = new QTextEdit;
    txted->setReadOnly(true);
    nextBlockSize = 0;
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(txted);
    setLayout(pvbxLayout);
}

Server::~Server()
{
}

void Server::slotNewConnection(){
    QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    txted->append("Client connected!");
}

void Server::slotReadClient(){
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {

            if (!nextBlockSize) {
                if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                    break;
                }
                in >> nextBlockSize;
            }

            if (pClientSocket->bytesAvailable() < nextBlockSize) {
                break;
            }

            quint8 command;
            in >> command;

            switch (command)
            {
                case 1:
                {
                    QString str;
                    in >> str;
                    QString strMessage = "Client name - " + str;
                    txted->append(strMessage);
                }
                break;
                case 2:
                {
                    QByteArray arrBlock;
                    QDataStream out(&arrBlock, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_2);
                    QString names = "";
                    for(QString str : repo->getNames()){
                        names.append(str);
                        names.append(",");
                    }
                    QString strMessage = "Client requested test names";
                    txted->append(strMessage);
                    out << quint16(0) << quint8(2) << names;
                    sendToClient(pClientSocket, arrBlock);
                }
                break;
                case 3:
                {
                    QByteArray arrBlock;
                    QDataStream out(&arrBlock, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_2);
                    QString names = "";
                    int id;
                    in >> id;
                    QString strMessage = "Client requested test with id:";
                    strMessage.append(QString::number(id));
                    txted->append(strMessage);
                    for(QString str : repo->getTasks(id+1)){
                        names.append(str);
                        names.append(";");
                    }
                    out << quint16(0) << quint8(3) << names;
                    sendToClient(pClientSocket, arrBlock);
                }
            }
            nextBlockSize = 0;
        }
    }

void Server::sendToClient(QTcpSocket *pSocket, QByteArray arrBlock){
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}
