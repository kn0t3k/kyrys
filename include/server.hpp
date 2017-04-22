#pragma once

#include <reference.hpp>

#include <QtNetwork/QSslSocket>
#include <QtCore/QMutex>

namespace Kyrys {
    class Server : public QTcpServer {
    Q_OBJECT
    private:

        std::map<int, QSslSocket *> m_loggedUsers;

        QMutex m_mutexLoggedVector;
        QMutex m_mutexFile;

    public:
        /**
         * @brief Construct a TCP server.
         * @param parent QT parent object.
         */
        explicit Server(QObject *parent = 0);

        /**
         * @brief Starts listening on an availible port.
         * @details This function searches for the closest availible port to the specified one
         * by adding +1 to the port number if the supplied port is not availible.
         * The first port that is found availible is used.
         *
         * @param port_no Port number.
         */
        void startServer(qint16 port_no);

        /**
         * Add user to the vector of logged users so we can later send messages to it.
         * @param ID ID of the user.
         * @param userSocket Socket through which we can communicate with the user.
         */
        void logUser(int ID, QSslSocket *const userSocket);

    protected:
        void incomingConnection(int descriptor);
    };
}
