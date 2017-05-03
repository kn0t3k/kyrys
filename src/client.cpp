#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>
#include <clientResolver.hpp>

#define CLIENT_PROMPT ">"
#define CHAT_PROMPT ">>"

using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;

typedef Kyrys::Enums::Client::Login::Status lStatus;                            //login status
typedef Kyrys::Enums::Client::Registration::Status rStatus;                     //registration status
typedef Kyrys::Enums::Client::Registration::PasswordSecQuality passwordQuality;
using Kyrys::Enums::Resolver::Status;

//Constructors
Client::Client(const QString &hostName, quint16 port)
        :
        QObject(0),
        m_socket(0),
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

//Getters
const Client::User &Client::getUser() const { return m_user; }

//Other methods
int Client::loadRegistrationCredentials(std::string &nickname, std::string &password, std::istream &in) {
    std::string nicknameBuffer;
    std::cout << "\nChoose nickname     :";
    std::getline(in, nicknameBuffer);

    std::string passwordBuffer1;
    std::string passwordBuffer2;

    for (int i = 0; i < 5; ++i) { //User has 5 tries to type password correctly twice in a row
        std::cout << "\nChoose password     :";
        std::getline(in, passwordBuffer1);
        if (controlPasswordSecQuality(passwordBuffer1) != passwordQuality::GOOD) {
            std::cout << "\nPassword is too short\nChoose password at least 8 characters long!" << std::endl;
            passwordBuffer1.clear();
            continue;
        } else {
            std::cout << "Repeat the password :";
            std::getline(in, passwordBuffer2);
            if (passwordBuffer1 != passwordBuffer2) {
                std::cout << "\nPasswords are not same!, please try it again" << std::endl;
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
    std::cout << "\nNow follows registration procedure" << std::endl;

    std::string nickname;
    std::string password;

    //Loading nickname and password
    if (loadRegistrationCredentials(nickname, password, in) == rStatus::BAD_PASSWORD) {
        std::cout << "\nRegistration failed. Check help page and try it again" << std::endl;
        return rStatus::BAD_PASSWORD;
    }

    //Hashing password
    const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), HASH);
    m_user = User(nickname, hashed_password);

    //Sending REGISTER_REQUEST
    QByteArray registerRequest = jsonMessageUserAuthentication(MessageType::REGISTER_REQUEST).toJson();
    if (!send(registerRequest)) {
        if (DEBUG) std::cout << "\nClient::registration - REGISTER_REQUEST message - FAIL" << std::endl;
        return rStatus::REQUEST_ERROR;
    }

    //Receiving REGISTER_RESPONSE
    QByteArray registerResponse = receive();
    if (registerResponse.isEmpty()) {
        if (DEBUG) std::cout << "\nClient::registration - REGISTER_RESPONSE message - FAIL" << std::endl;
        return rStatus::RESPONSE_ERROR;
    }

    //Parsing REGISTER_RESPONSE message
    ClientResolver clientResolver;
    int returnState = clientResolver.parse(registerResponse);

    if (returnState == Status::FAILED)
        return rStatus::SERVER_ERROR;
    else {
        if (DEBUG) std::cout << "\ngetSuccess = " << clientResolver.getItem().getSuccess() << std::endl;
        if (clientResolver.getItem().getSuccess()) {
            copyRegistrationItem(clientResolver.getItem());
            std::cout << "\nNew user was succesfully registered\n" << std::endl;
            m_user.printUser();
        } else {
            std::cout << "\nRegistration failed" << std::endl;
            return rStatus::SERVER_ERROR;
        }
    }
    return rStatus::SUCCESS;
}

int Client::login(std::istream &in) {
    std::string nickname;
    std::string password;

    for (int i = 0; i < 5; ++i) {
        //Loading login credentials
        loadLoginCredentials(nickname, password, in);

        //Hashing password
        QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), HASH);
        m_user = User(nickname, hashed_password);
        hashed_password.clear();

        //Sending LOGIN_REQUEST
        QByteArray loginRequest = jsonMessageUserAuthentication(
                MessageType::LOGIN_REQUEST).toJson(); //this is message for server
        if (!send(loginRequest)) {
            if (DEBUG)std::cout << "\nClient::registration - LOGIN_REQUEST message - FAIL" << std::endl;
            return lStatus::REQUEST_ERROR;
        }

        //Receiving REGISTER_RESPONSE
        QByteArray loginResponse = receive();
        qDebug() << "login response: " << loginResponse;
        if (loginResponse.isEmpty()) {
            if (DEBUG)std::cout << "\nClient::registration - LOGIN_RESPONSE message - FAIL" << std::endl;
            return lStatus::RESPONSE_ERROR;
        }

        //Parsing LOGIN_RESPONSE message
        ClientResolver clientResolver;
        int returnState = clientResolver.parse(loginResponse);

        if (returnState == Status::FAILED) {
            return lStatus::SERVER_ERROR;
        } else {
            if (DEBUG)std::cout << "\ngetSuccess = " << clientResolver.getItem().getSuccess() << std::endl;
            if (clientResolver.getItem().getSuccess()) {
                std::cout << "\nUser: " << m_user.getNickname() << "was succesfully logged in" << std::endl;
                return lStatus::SUCCESS;
            } else {
                std::cout << "\nLogin failed - try it again" << std::endl;
                continue;
            }
        }
    }
    return lStatus::FAIL;
}

QJsonDocument Client::jsonMessageUserAuthentication(MessageType messageType) {
    QJsonObject args_obj;
    QJsonObject root_obj;

    if (messageType == MessageType::REGISTER_REQUEST) {
        root_obj["messageType"] = "REGISTER_REQUEST";
        root_obj["method"] = "register";
    } else {
        root_obj["messageType"] = "LOGIN_REQUEST";
        root_obj["method"] = "login";
    }

    args_obj["nickname"] = QString::fromStdString(m_user.getNickname());
    args_obj["password"] = m_user.getPasswordHash().toStdString().c_str();
    args_obj["hash algorithm"] = HASH;

    root_obj.insert("args", args_obj);

    // todo add - salt of random values at the end of JSON message
    //		    - ID of message (maybe)

    return QJsonDocument(root_obj);
}

void Client::run(std::istream &in) {
    QtConcurrent::run(this, &Client::threadRun);
}

void Client::copyRegistrationItem(const Item &item) {
    m_user.setID(item.getID());
    m_user.setNickname(item.getNick().toStdString());
}


void Client::messageIncoming() {
    qDebug() << "something received";
    if (m_isChat) {
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

void Client::runChat(std::istream &in) {
    std::string command;
    m_isChat = true;

    do {
        std::cout << "\n" << CHAT_PROMPT << " " << std::flush;
        std::getline(in, command);

        if (command == "#callid") {
            //todo
            continue;
        }

        if (command == "#callnick") {
            //todo
            continue;
        }

        if (command == "#history") {
            //todo
            continue;
        }

        if (command == "#sendto") {
            sendTo();
            //todo: ak je druhy klient v chat interface, tak sa mu pernamentne bez nadviazavania spojenia zobrazi sprava
            continue;
        }

        if (command == "#quit") {
            m_isChat = false;
            break;
        }

        std::cout << "\nUNKNOWN CHAT COMMAND\n" << std::endl;
    } while (command != "#quit");
}

void Client::sendTo() {

    //Preparing fromID, toID and inicializing Chat class
    std::cout << "\n" << CHAT_PROMPT << " to ID:" << std::flush;
    unsigned int toID;
    std::string buffer;
    std::getline(std::cin, buffer);
    toID = static_cast<unsigned int>(std::stoul(buffer));
    Friend sender(m_user.getID());
    Friend receiver(toID);
    Chat chat(sender, receiver);

    //Preparing CHAT_DATA message and sending it to server
    std::cout << "\n" << CHAT_PROMPT << " [You]: " << std::flush;
    getline(std::cin, buffer);
    QString chatData = chat.jsonCreateChatData(sender, receiver, QString::fromStdString(buffer)).toJson();
    send(chatData);
}

void Client::printMessage(const Item &incomingMessage) {
    if (incomingMessage.isValid())
        std::cout << "\n" << CHAT_PROMPT << " [FROM ID " << incomingMessage.getFromID() << "]: "
                  << incomingMessage.getData().toStdString() << std::endl;
}

void Client::threadRun() {
    std::string command;
    do {
        std::cout << "\n> " << std::flush;
        std::getline(std::cin, command);
        if (command == "help") {
            //todo: writes some man page on stdout, use welcomepages.hpp
            continue;
        }
        if (command == "register") {
            registration();
            continue;
        }
        if (command == "login") {
            login();
            continue;
        }
        if (command == "chat") {
            runChat();
            continue;
        }

        std::cout << "\nUNKNOWN COMMAND\n" << std::endl;
    } while (command != "quit");
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
