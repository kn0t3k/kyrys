#include <reference.hpp>
#include <socketThread.hpp>
#include <resolver.hpp>

using Kyrys::Enums::Resolver::Mode;
using Kyrys::Resolver;
using Kyrys::SocketThread;

SocketThread::SocketThread(int socketID, QObject *parent) :
    QThread(parent),
    m_socketID(socketID) {
    qDebug() << "Constructing new thread";

}

void SocketThread::encrypted() {
    qDebug() << "encrypted";
    connect( m_socket, SIGNAL(readyRead()),
             this, SLOT(readData()), Qt::DirectConnection);
}

void SocketThread::run() {
    qDebug() << "run";

    m_socket = new QSslSocket(NULL);

    m_socket->setProtocol(QSsl::SslV3);

    QByteArray key;
    QByteArray cert;

    QFile file_key("ia.key");
    if (file_key.open(QIODevice::ReadOnly))
    {
        key = file_key.readAll();
        file_key.close();
    }
    else
    {
        qDebug() << file_key.errorString();
    }

    QFile file_cert("ia.crt");
    if (file_cert.open(QIODevice::ReadOnly))
    {
        cert = file_cert.readAll();
        file_cert.close();
    }
    else
    {
        qDebug() << file_cert.errorString();
    }


    QSslKey ssl_key(key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "server");

    QSslCertificate ssl_cert(cert);

    m_socket->addCaCertificate(ssl_cert);
    m_socket->setLocalCertificate(ssl_cert);
    m_socket->setPrivateKey(ssl_key);


    connect( m_socket, SIGNAL(sslErrors(QList<QSslError>)),
             this, SLOT(sslError(QList<QSslError>)) );

    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, true );

    if (!m_socket->setSocketDescriptor(m_socketID)) {
        qDebug() << "descriptor fail";
        return;
    } else {
        connect(m_socket, SIGNAL(encrypted()), this, SLOT(encrypted()), Qt::DirectConnection);
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(quit()));
        m_socket->startServerEncryption();
        qDebug() << "client connected";
        exec();
    }
}

void SocketThread::readData() {
    qDebug() << "thread Reading data:" << QString::fromUtf8(m_socket->readAll());
    //cal resolver and communicate with client
}

void SocketThread::quit() {
    qDebug() << "thread quit" << m_socketID;;
    m_socket->deleteLater();
    exit(0);
}

void SocketThread::sslError(QList<QSslError> errors) {
    foreach (const QSslError &e, errors){
        qDebug() << "Server error:\t" << e.errorString();
    }
}