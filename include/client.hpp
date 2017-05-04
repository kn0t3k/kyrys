#pragma once

#include <reference.hpp>
#include <user.hpp>
#include <istream>
#include <item.hpp>
#include <chat.hpp>

namespace Kyrys {
    class Client : public QObject {
     Q_OBJECT

        typedef Kyrys::User User;
        typedef std::string string;
        typedef Kyrys::Enums::JsonMessage::MessageType MessageType;
        typedef Kyrys::Chat Chat;

     private:
        QSslSocket *m_socket = nullptr;
        User m_user;
        QString m_hostname;
        quint16 m_port;
        bool m_isAsync = false;

     signals:

        void clientFinished();

     private slots:

        void socketEncrypted();

        void sslErrors(const QList<QSslError> &errors);

        void socketError(QAbstractSocket::SocketError error);

        void messageIncoming();

        /**
         * @brief Connect to the specified server.
         * @details Sets up a connection between a client and server, compares the server-supplied
         * certificate to the one distributed with the client to check server authority.
         * @return Return true if connection was succesfull.
         */
        void secureConnect();

        /**
       * @brief run	It's a method executing commands from Client commandline.
       * @param in 	In is parameter for pushing fake input instead of standard input from std::cin. Use this param for testing purpose. Default value is std::cin
       */
        void run(std::istream &in = std::cin);

        void toggleIsAsync();

        void clientQuit();

     public:
        //Constructors
        /**
        * @brief Client constructor.
        * @details Constructs a new client and prepares it to connect to a server.
        *
        * @param hostName Hostname of the server to connect to.
        * @param port Port at which the server listens.
        * @param parent QT parent object.
        */
        explicit Client(const QString &hostName, quint16 port);

        //Destructors
        /**
         * @brief Client destructor.
         * @details De-allocates the used memory.
         */
        ~Client();

        /**
         *	@brief send message to socket m_socket and control if data was send succesfully
         *	@note	expecting that socket is already connected to server, if not method will fail
         *	@return true if successfully send data, false otherwise
         */
        bool send(const QString &data);

        /**
         *	@brief receive message from socket m_socket and control if data was received succesfully
         *	@param [out]response	reference to buffer response where will be written data from socket
         *	@return true if successfully received data, false if no data arrived in set timeout 1000ms
         *	@note	expecting that socket is already connected to server, if not method will fail
         */
        QByteArray receive();

     public slots:

        bool writeToSocket(const QByteArray &data);

        QByteArray readFromSocket();
    };
}
