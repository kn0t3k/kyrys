#include <reference.hpp>
#include <server.hpp>
#include <socketThread.hpp>


using Kyrys::Server;
using Kyrys::SocketThread;


Server::Server(QObject *parent)
        :
        QTcpServer(parent) {
    m_loggedUsers = {};
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

    SocketThread *thr = new SocketThread(descriptor, "C:\\__TEMP__\\", &m_mutexFile, this);
    connect(thr, SIGNAL(finished()), thr, SLOT(deleteLater()));
    thr->start();
}

void Server::logUser(int ID, QSslSocket *const userSocket) {
    qDebug() << "logging in: " << ID;
    m_mutexLoggedVector.lock();
    if (m_loggedUsers.find(ID) != m_loggedUsers.end()) {    // if user is already logged in, replace its socket
        m_loggedUsers.erase(ID);
    }
    m_loggedUsers.insert(std::make_pair(ID, userSocket));
    for (auto &item : m_loggedUsers) {
        std::cout << "logged: " << item.first << std::endl;
    }
    m_mutexLoggedVector.unlock();
}

QSslSocket *Server::getUserSocket(int ID) {
    auto userSocketIterator = m_loggedUsers.find(ID);
    if(userSocketIterator == m_loggedUsers.end()){
        return nullptr;
    }
    return userSocketIterator->second;
}

