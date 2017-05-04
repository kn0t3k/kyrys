#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>

#include <clientResolver.hpp>
#include <chatInterface.hpp>


using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;
using Kyrys::Enums::Resolver::Status;

using Kyrys::ChatInterface;

//Constructors
Client::Client(const QString &hostName, quint16 port)
        :
        QObject(nullptr),
        m_socket(nullptr),
        m_user(),
        m_hostname(hostName),
        m_port(port) {}

//Destructors
Client::~Client() { delete m_socket; }

//Network connectivity methods
void Client::secureConnect() {
    if (!m_socket) {
        m_socket = new QSslSocket(this);
        m_socket->setProtocol(QSsl::SslV3);
        connect(m_socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
    }
    qDebug() << "connecting to host";

    QFile certFile("ia.crt");
    Q_ASSERT(certFile.open(QIODevice::ReadOnly));
    QSslCertificate cert(&certFile, QSsl::Pem);
    m_socket->addCaCertificate(cert);

    m_socket->connectToHostEncrypted(m_hostname, m_port);
    if (m_socket->waitForEncrypted(10000)) {
        qDebug() << "connected";
        run();
    } else {
        emit clientFinished();
    }
}

void Client::socketError(QAbstractSocket::SocketError) {
    qDebug() << "client error: " << m_socket->errorString();
}

void Client::socketEncrypted() {
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(messageIncoming()));
}

void Client::sslErrors(const QList<QSslError> &errors) {
    qDebug() << "Client error:\t" << errors;
}

bool Client::send(const QString &data) {
    bool retval = false;
    QMetaObject::invokeMethod(this, "writeToSocket", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retval),
                              Q_ARG(QByteArray, data.toLocal8Bit()));
    return retval;
}

QByteArray Client::receive() {
    QByteArray retval;
    QMetaObject::invokeMethod(this, "readFromSocket",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QByteArray, retval));
    return retval;
}

void Client::run(std::istream &in) {
    QThread *chatInterfaceThread = new QThread;
    ChatInterface *interface = new ChatInterface(this);

    interface->moveToThread(chatInterfaceThread);

    connect(chatInterfaceThread, SIGNAL(finished()), interface, SLOT(deleteLater()));
    connect(chatInterfaceThread, SIGNAL(started()), interface, SLOT(commandLoop()));
    connect(interface, SIGNAL(toggleAsyncSocketRead()), this, SLOT(toggleIsAsync()));
    connect(interface, SIGNAL(clientChatInterfaceQuit()), interface, SLOT(deleteLater()));
    connect(interface, SIGNAL(clientChatInterfaceQuit()), this, SLOT(clientQuit()));

    chatInterfaceThread->start();
}

void Client::messageIncoming() {
    qDebug() << "something received";
    if (m_isAsync) {
        qDebug() << "client chat message received:\n" << m_socket->readAll();
    }

    //Receiving some CHAT message: it is CHAT_RESPONSE or CHAT_DATA
    //QByteArray incomingMessage;
    /*if (receive(incomingMessage)) {
        if (DEBUG)
            std::cout << "\nClient::CHAT - CHAT message arrived - size: " << incomingMessage.length() << std::endl;
    } else {
        if (DEBUG)std::cout << "\nClient::CHAT - CHAT message - FAIL" << std::endl;
    }*/
/*
    //Starting parser
    ClientResolver clientResolver;
    int returnState = 0;//clientResolver.parse(incomingMessage);


    //Parsing received message
    if (returnState == Status::SUCCESS) {
        if (clientResolver.getItem().getMessageType() == MessageType::CHAT_RESPONSE) {
            //todo
        }
        if (clientResolver.getItem().getMessageType() == MessageType::CHAT_DATA) {
            printMessage(clientResolver.getItem());
        }
    } else {
        if (DEBUG)std::cout << "Client::messageIncoming: FAIL" << std::endl;
        return;
    }
    */
}

bool Client::writeToSocket(const QByteArray &data) {
    m_socket->write(data);
    if (!m_socket->waitForBytesWritten(500)) {
        return false;
    } else {
        return true;
    }
}

QByteArray Client::readFromSocket() {
    if (m_socket->waitForReadyRead(500)) {
        return m_socket->readAll(); //REGISTER_RESPONSE received
    }
    return QByteArray();
}

void Client::toggleIsAsync() {
    m_isAsync = !m_isAsync;
}

void Client::clientQuit() {
    std::cout << "quitting" << std::endl;
    m_socket->close();
    emit clientFinished();
}
