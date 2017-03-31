#include <reference.hpp>
#include <server.hpp>

using Kyrys::Server;
using Kyrys::Enums::Resolver::Mode;

Server::Server(unsigned port, QObject *parent) : QObject(parent), mResolver("") {
    mServer = new QTcpServer(parent);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!mServer->listen(QHostAddress::Any, port)) {
        std::cerr << "server could not start" << std::endl;
        delete mServer;
    } else {
        qDebug() << "server started at: " << port;
    }
}

void Server::newConnection() {
    qDebug() << "new connection";
    QTcpSocket *socket = mServer->nextPendingConnection();

    // receive data from client

    // resolve this data,
    // check return value if client's request was succesful

    // just for example, works with telnet
    socket->write("OK");
    socket->flush();
    socket->waitForBytesWritten(3000);

    socket->close();
}