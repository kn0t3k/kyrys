#include <reference.hpp>
#include <socketThread.hpp>

using Kyrys::SocketThread;

using Kyrys::Enums::Resolver::Mode;

SocketThread::SocketThread(int socketID, Server *parent)
        :
        m_socketID(socketID),
        m_resolver(),
        m_server(parent) {

}

void SocketThread::encrypted() {
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(readData()), Qt::DirectConnection);
}

void SocketThread::run() {
    m_socket = new QSslSocket(NULL);

    m_socket->setProtocol(QSsl::TlsV1SslV3);

    QByteArray key;
    QByteArray cert;

    QFile file_key("ia.key");
    if (file_key.open(QIODevice::ReadOnly)) {
        key = file_key.readAll();
        file_key.close();
    } else {
        qDebug() << file_key.errorString();
        emit socketThreadFinished();
        return;
    }

    QFile file_cert("ia.crt");
    if (file_cert.open(QIODevice::ReadOnly)) {
        cert = file_cert.readAll();
        file_cert.close();
    } else {
        qDebug() << file_cert.errorString();
        emit socketThreadFinished();
        return;
    }

    QSslKey ssl_key(key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "server");

    QSslCertificate ssl_cert(cert);

    m_socket->addCaCertificate(ssl_cert);
    m_socket->setLocalCertificate(ssl_cert);
    m_socket->setPrivateKey(ssl_key);

    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslError(QList<QSslError>)));

    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    if (!m_socket->setSocketDescriptor(m_socketID)) {
        qDebug() << "descriptor fail";
        emit socketThreadFinished();
    } else {
        connect(m_socket, SIGNAL(encrypted()), this, SLOT(encrypted()), Qt::DirectConnection);
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(quit()));
        m_socket->startServerEncryption();
        qDebug() << "client connected";
    }
}

void SocketThread::readData() {
    QString incomingData = QString::fromUtf8(m_socket->readAll());
    int result = m_resolver.parse(incomingData, Mode::USE_JSON);
    qDebug() << "parse result: " << result;

    if (m_resolver.isLogin()) {
        m_server->logUser(m_resolver.getItem().getID(), this);
        sendData(m_resolver.prepareResponse());
    } else if (m_resolver.isForward()) {
        auto otherUser = m_server->getTargetUserThread(m_resolver.getRecipientID());

        if (otherUser != nullptr) {
            QMetaObject::invokeMethod(otherUser, "sendData", Qt::QueuedConnection,
                                      Q_ARG(QByteArray, m_resolver.prepareResponse()));
        }
    } else {
        sendData(m_resolver.prepareResponse());
    }
}

void SocketThread::quit() {
    m_socket->deleteLater();
    m_server->logOut(m_resolver.getItem().getID());
    emit socketThreadFinished();
}

void SocketThread::sslError(QList<QSslError> errors) {
    qDebug() << "Server error:\t" << errors;
}

void SocketThread::sendData(const QByteArray &data) {
    m_socket->write(data);
    if (!m_socket->waitForBytesWritten(300)) {
        qDebug() << "socket error?";
    }
}
