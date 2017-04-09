#include <reference.hpp>
#include <server.hpp>

using Kyrys::Server;
using Kyrys::Enums::Resolver::Mode;


Server::Server(unsigned port, QObject *parent) : QObject(parent), m_resolver("C:\\__TEMP__\\") {
    m_server = new QTcpServer(parent);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!m_server->listen(QHostAddress::Any, port)) {
        std::cerr << "server could not start" << std::endl;
        delete m_server;
    } else {
        qDebug() << "server started at: " << port;
    }
}

void Server::newConnection() {
    qDebug() << "new connection";
    QTcpSocket *socket = m_server->nextPendingConnection();

    // receive data from client

    // resolve this data,
    // check return value if client's request was succesful

    // just for example, works with telnet
    socket->write("OK");
    socket->flush();
    socket->waitForBytesWritten(3000);

    socket->close();
}
