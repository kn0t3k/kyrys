#include <reference.hpp>
#include <server.hpp>
#include <socketThread.hpp>


using Kyrys::Server;
using Kyrys::SocketThread;


Server::Server(QObject *parent) : QTcpServer(parent) {
    m_loggedUsers = {};
}

void Server::startServer(quint16 port_no) {
    if (!listen(QHostAddress::Any, port_no)) {
        qDebug() << "server could not start";
    } else {
        qDebug() << "listening on: " << port_no;
    }
}

void Server::incomingConnection(int descriptor) {
    QThread *thread = new QThread;
    SocketThread *worker = new SocketThread(descriptor, this);

    worker->moveToThread(thread);
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), worker, SLOT(run()));
    connect(worker, SIGNAL(socketThreadFinished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(socketThreadFinished()), thread, SLOT(quit()));

    thread->start();
}

void Server::logUser(int ID, SocketThread *const userSocket) {
    if (m_loggedUsers.find(ID) != m_loggedUsers.end()) {    // if user is already logged in, replace its socket
        m_loggedUsers.erase(ID);
    }
    m_loggedUsers.insert(std::make_pair(ID, userSocket));
}

SocketThread *Server::getTargetUserThread(int ID) {
    auto userIterator = m_loggedUsers.find(ID);
    if (userIterator == m_loggedUsers.end()) {
        return nullptr;
    }
    return userIterator->second;
}

void Server::logOut(int ID) {
    m_loggedUsers.erase(ID);
}

