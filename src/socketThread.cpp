#include <reference.hpp>
#include <socketThread.hpp>

using Kyrys::SocketThread;

using Kyrys::Enums::Resolver::Mode;

SocketThread::SocketThread(int socketID, const QString &resolverPath, QMutex *const mutexFile, Server *parent)
        :
        QThread(dynamic_cast<QObject *>(parent)),
        m_socketID(socketID),
        m_resolver(mutexFile),
        m_server(parent) {

}

void SocketThread::encrypted() {
    qDebug() << "encrypted";
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
        return;
    }

    QFile file_cert("ia.crt");
    if (file_cert.open(QIODevice::ReadOnly)) {
        cert = file_cert.readAll();
        file_cert.close();
    } else {
        qDebug() << file_cert.errorString();
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
    } else {
        connect(m_socket, SIGNAL(encrypted()), this, SLOT(encrypted()), Qt::DirectConnection);
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(quit()));
        m_socket->startServerEncryption();
        qDebug() << "client connected";
        exec();
    }
}

void SocketThread::readData() {
    QString incomingData = QString::fromUtf8(m_socket->readAll());
    int result = m_resolver.parse(incomingData, Mode::USE_JSON);
    qDebug() << "parse result: " << result;

    if (m_resolver.isLogin()) {
        m_server->logUser(m_resolver.getItem().getID(), m_socket);
        sendData(m_resolver.prepareResponse());
    } else if (m_resolver.isForward()) {
        auto userSocket = m_server->getUserSocket(m_resolver.getRecipientID());
        if(userSocket == nullptr){
            qDebug() << "user not logged in";
        } else {
            qDebug() << "forwarding to ..." << m_resolver.getRecipientID();
            userSocket->write(m_resolver.prepareResponse());
        }
    } else {
        sendData(m_resolver.prepareResponse());
    }

}

void SocketThread::quit() {
    qDebug() << "thread quit" << m_socketID;;
    m_socket->deleteLater();
    exit(0);
}

void SocketThread::sslError(QList<QSslError> errors) {
            foreach (const QSslError &e, errors) {
            qDebug() << "Server error:\t" << e.errorString();
        }
}

void SocketThread::sendData(const QString &data) {
    m_socket->write(data.toLatin1().data());
    if (m_socket->waitForBytesWritten(300)) {
        qDebug() << "success";
    } else {
        qDebug() << "fail" << m_socket->errorString();
    }
}
