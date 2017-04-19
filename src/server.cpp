#include <reference.hpp>
#include <server.hpp>
#include <socketThread.hpp>


using Kyrys::Server;
using Kyrys::SocketThread;


Server::Server(QObject *parent) :
    QTcpServer(parent) {

}

void Server::startServer(qint16 port_no) {
    if (!listen(QHostAddress::Any, port_no)) {
        qDebug() << "server could not start";
    } else {
        qDebug() << "listening on: " << port_no;
    }
    
}

void Server::incomingConnection(int descriptor) {
    qDebug() << "conneciton incoming" << descriptor;

    SocketThread *thr = new SocketThread(descriptor, this);
    connect(thr, SIGNAL(finished()), thr, SLOT(deleteLater()));
    thr->start();
}

