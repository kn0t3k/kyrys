#pragma once

#include <reference.hpp>
#include <serverResolver.hpp>
#include <server.hpp>

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtCore/QMutex>


namespace Kyrys {
    class SocketThread : public QThread {
        typedef Kyrys::ServerResolver Resolver;
        typedef Kyrys::Server Server;
    Q_OBJECT

    private:

        int m_socketID;

        QSslSocket *m_socket = nullptr;

        Resolver m_resolver;

        Server *m_server = nullptr;


    private slots:

        void readData();

        void sendData(const QString &data);

        void quit();

        void encrypted();

    public:
        /**
         * @brief Construct a thread, that will represent a connection between client and server.
         * This is a key concept since the server will be able to server multiple clients simultaneously.
         * @param socketID Socket descriptor of the new connection.
         * @param parent QT parent object.
         */
        explicit SocketThread(int socketID, const QString &resolverPath, QMutex *const mutexFile, Server *parent = 0);

        /**
         * @brief Starts the execution of the newly created thread.
         * That means communicating with the client, sending responses etc.
         */
        void run() override;

    public slots:

        void sslError(QList<QSslError> errors);

    };

}