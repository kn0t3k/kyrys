#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>

using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;

typedef Kyrys::Enums::Client::Login::Status lStatus;                            //login status
typedef Kyrys::Enums::Client::Registration::Status rStatus;                     //registration status
typedef Kyrys::Enums::Client::Registration::PasswordSecQuality passwordQuality;



Client::Client(const QString &hostName, quint16 port, QObject *parent) :
        QObject(parent),
        m_socket(0),
        m_user(),
        m_hostname(hostName),
        m_port(port) {}

bool Client::secureConnect() {
    if (!m_socket) {
        qDebug() << "new socket";
        m_socket = new QSslSocket(this);
        m_socket->setProtocol(QSsl::SslV3);
        connect(m_socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
        connect(m_socket, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
    }
    qDebug() << "connecting to host";

    QFile certFile("ia.crt");
    Q_ASSERT(certFile.open(QIODevice::ReadOnly));
    QSslCertificate cert(&certFile, QSsl::Pem);
    m_socket->addCaCertificate(cert);

    m_socket->connectToHostEncrypted(m_hostname, m_port);
    if (m_socket->waitForEncrypted(10000)) {
        qDebug() << "connected";
        return true;
    } else {
        return false;
    }
}

bool Client::sendData(const QString &data) {
    m_socket->write(data.toLatin1().data());
    return m_socket->waitForBytesWritten(200);
}

void Client::socketError(QAbstractSocket::SocketError) {
    qDebug() << "client error: " << m_socket->errorString();

}

void Client::socketReadyRead() {
    qDebug() << "client received: " << QString::fromUtf8(m_socket->readAll());
}

void Client::socketEncrypted() {
    if (!m_socket) {
        return;
    }
    qDebug() << "socketEncrypted";
}

void Client::sslErrors(const QList<QSslError> &errors) {
            foreach (const QSslError &e, errors) {
            qDebug() << "Client error:\t" << e.errorString();
        }
}

//Getters
const Client::User &Client::getUser() const { return m_user; }


//Other methods
int Client::loadRegistrationCredentials(std::string &nickname, std::string &password, std::istream &in) {
    std::string nicknameBuffer;
    std::cout << "Choose nickname: ";
    std::getline(in, nicknameBuffer);

    std::string passwordBuffer1;
    std::string passwordBuffer2;

    for (int i = 0; i < 5; ++i) { //User has 5 tries to type password correctly twice in a row
        std::cout << "\nChoose password: ";
        std::getline(in, passwordBuffer1);
        if (controlPasswordSecQuality(passwordBuffer1) != passwordQuality::GOOD) {
            std::cout << "\nPassword is too short\nChoose password at least 8 characters long!" << std::endl;
            passwordBuffer1.clear();
            continue;
        } else {
            std::cout << "\nRepeat the password:";
            std::getline(in, passwordBuffer2);
            if (passwordBuffer1 != passwordBuffer2) {
                std::cout << "Passwords are not same!, please try it again" << std::endl;
                passwordBuffer1.clear();
                passwordBuffer2.clear();
                continue;
            } else {
                nickname = nicknameBuffer;
                password = passwordBuffer1;
                passwordBuffer1.clear(); //security precautions
                passwordBuffer2.clear();
                return rStatus::SUCCESS;
            }
        }
    }
    return rStatus::BAD_PASSWORD;
}

int Client::loadLoginCredentials(std::string &nickname, std::string &password, std::istream &in) {
    std::cout << "Nickname: ";
    std::getline(in, nickname);
    std::cout << "\nPassword: ";
    std::getline(in, password);
    return 0;
}

int Client::controlPasswordSecQuality(const std::string &password) const {
    if (password.length() >= 8)
        return passwordQuality::GOOD;
    else
        return passwordQuality::TOO_SHORT;
}

int Client::registration(std::istream &in) {
    rStatus status = rStatus::REGISTRATION_STARTED;
    QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;

    std::cout << "Now follows registration procedure" << std::endl;

    std::string nickname;
    std::string password;

    if (loadRegistrationCredentials(nickname, password, in) == rStatus::BAD_PASSWORD) {
        std::cout << "Registration failed. Check help page and try it again" << std::endl;
        return status = rStatus::BAD_PASSWORD;
    } else {
        status = rStatus::CREDENTIALS_LOADED;
    }

    const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
    m_user = User(nickname, hashed_password, usedHashAlgorithm);
    status = rStatus::PASSWORD_HASHED;

/* //This part of code crushing program
	m_socket->write(jsonMessageUserAuthentication(MessageType::REGISTER_REQUEST).toJson()); //REGISTER_REQUEST message was send
	m_socket->waitForBytesWritten();
	if(!m_socket->waitForReadyRead()){
		return status = rStatus::SERVER_ERROR;
	} else {
		QJsonDocument response = QJsonDocument::fromJson(m_socket->readAll());
		//todo: parse data from socket
	}
*/

    //todo: call server with json message and end with 0 after server answers that registration is succesfully finished on his side
    //I expect something like jsonMessageUserAuthentication -> Socket -> Server
    //Server -> Socket -> jsonMessageRegisterResponse(OK / not OK)

    return status = rStatus::SUCCESS;
}

int Client::login(std::istream &in) {
    lStatus status = lStatus::LOGIN_STARTED;
    std::string nickname;
    std::string password;
    QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;

    //cyclus begin
    loadLoginCredentials(nickname, password, in);
    status = lStatus::CREDENTIALS_LOADED;

    QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
    m_user = User(nickname, hashed_password, usedHashAlgorithm);
    hashed_password.clear();
    status = lStatus::PASSWORD_HASHED;

    QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::LOGIN_REQUEST); //this is message for server
    //cyclus end

    //TODO - contact server with JSON message obtaining hash of logging in user

    return status = lStatus::SUCCESS;
}

QJsonDocument Client::jsonMessageUserAuthentication(MessageType messageType) {
    QJsonObject args_obj;
    QJsonObject root_obj;
    QString key;
    QString value;
    if (messageType == MessageType::REGISTER_REQUEST) {
        root_obj["messageType"] = "REGISTER_REQUEST";
        root_obj["method"] = "register";
    } else {
        root_obj["messageType"] = "LOGIN_REQUEST";
        root_obj["method"] = "login";
    }

    args_obj["nickname"] = QString::fromStdString(m_user.getNickname());
    args_obj["password"] = m_user.getPasswordHash().toStdString().c_str();
    args_obj["hash algorithm"] = m_user.getUsedHashAlgorithm();

    root_obj.insert("args", args_obj);

    // todo add - salt of random values at the end of JSON message
    //		    - ID of message (maybe)

    return QJsonDocument(root_obj);
}

void Client::run(std::istream &in) {
    std::string command;
    do {
        std::getline(in, command);

    } while (command != "quit");
}

Client::~Client() { delete m_socket; }
