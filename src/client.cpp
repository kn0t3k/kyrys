#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>
#include <clientResolver.hpp>

using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;

typedef Kyrys::Enums::Client::Login::Status lStatus;                            //login status
typedef Kyrys::Enums::Client::Registration::Status rStatus;                     //registration status
typedef Kyrys::Enums::Client::Registration::PasswordSecQuality passwordQuality;
using Kyrys::Enums::Resolver::Status;

#define CLIENT_PROMPT >

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
    rStatus status = rStatus::REGISTRATION_STARTED;
    QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;

    std::cout << "\nNow follows registration procedure" << std::endl;

    std::string nickname;
    std::string password;


	//Loading nickname and password
    if (loadRegistrationCredentials(nickname, password, in) == rStatus::BAD_PASSWORD) {
        std::cout << "\nRegistration failed. Check help page and try it again" << std::endl;
        return status = rStatus::BAD_PASSWORD;
    } else {
        status = rStatus::CREDENTIALS_LOADED;
    }


	//Hashing password
    const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
    m_user = User(nickname, hashed_password, usedHashAlgorithm);
    status = rStatus::PASSWORD_HASHED;


	//Sending REGISTER_REQUEST
	QByteArray registerRequest = jsonMessageUserAuthentication(MessageType::REGISTER_REQUEST).toJson();
	if(send(registerRequest)) {
		if(DEBUG)std::cout << "\nClient::registration - REGISTER_REQUEST message send - size: " << registerRequest.length() << std::endl;
	} else {
		if(DEBUG)std::cout << "\nClient::registration - REGISTER_REQUEST message - FAIL" << std::endl;
		return status = rStatus::REQUEST_ERROR;
	}


	//Receiving REGISTER_RESPONSE
	QByteArray registerResponse;
	if(receive(registerResponse)) {
		if(DEBUG)std::cout << "\nClient::registration - REGISTER_RESPONSE message arrived - size: " << registerResponse.length() << std::endl;
	} else {
		if(DEBUG)std::cout << "\nClient::registration - REGISTER_RESPONSE message - FAIL" << std::endl;
		return status = rStatus::RESPONSE_ERROR;
	}


	//Parsing REGISTER_RESPONSE message
	ClientResolver clientResolver;
	int returnState = clientResolver.parse(registerResponse);

	if(returnState == Status::FAILED)
		return rStatus::SERVER_ERROR;
	else{
		if(DEBUG)std::cout << "\ngetSuccess = " << clientResolver.getItem().getSuccess() << std::endl;
		if(clientResolver.getItem().getSuccess()){
			copyRegistrationItem(clientResolver.getItem());
			std::cout << "\nNew user was succesfully registered\n" << std::endl;
			m_user.printUser();
		} else {
			std::cout << "\nRegistration failed" << std::endl;
			return rStatus::SERVER_ERROR;
		}
	}
    return status = rStatus::SUCCESS;
}

int Client::login(std::istream &in) {
    lStatus status = lStatus::LOGIN_STARTED;
    std::string nickname;
    std::string password;
    QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;


	for(int i = 0; i < 5; ++i) {
		//Loading login credentials
		loadLoginCredentials(nickname, password, in);
		status = lStatus::CREDENTIALS_LOADED;


		//Hashing password
		QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
		m_user = User(nickname, hashed_password, usedHashAlgorithm);
		hashed_password.clear();
		status = lStatus::PASSWORD_HASHED;


		//Sending LOGIN_REQUEST
		QByteArray loginRequest = jsonMessageUserAuthentication(MessageType::LOGIN_REQUEST).toJson(); //this is message for server
		if(send(loginRequest)) {
			if(DEBUG)std::cout << "\nClient::login - LOGIN_REQUEST message send - size: " << loginRequest.length() << std::endl;
		} else {
			if(DEBUG)std::cout << "\nClient::registration - LOGIN_REQUEST message - FAIL" << std::endl;
			return status = lStatus::REQUEST_ERROR;
		}


		//Receiving REGISTER_RESPONSE
		QByteArray loginResponse;
		if(receive(loginResponse)) {
			if(DEBUG)std::cout << "\nClient::registration - LOGIN_RESPONSE message arrived - size: " << loginResponse.length() << std::endl;
		} else {
			if(DEBUG)std::cout << "\nClient::registration - LOGIN_RESPONSE message - FAIL" << std::endl;
			return status = lStatus::RESPONSE_ERROR;
		}


		//Parsing LOGIN_RESPONSE message
		ClientResolver clientResolver;
		int returnState = clientResolver.parse(loginResponse);

		if(returnState == Status::FAILED)
			return lStatus::SERVER_ERROR;
		else{
			if(DEBUG)std::cout << "\ngetSuccess = " << clientResolver.getItem().getSuccess() << std::endl;
			if(clientResolver.getItem().getSuccess()){
				std::cout << "\nUser: " << m_user.getNickname() << "was succesfully logged in" << std::endl;
				return status = lStatus::SUCCESS;
			} else {
				std::cout << "\nLogin failed - try it again" << std::endl;
				continue;
			}
		}
	}
    return status = lStatus::FAIL;
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
        std::cout << "\n> " << std::flush;
        std::getline(in, command);
		if(command == "register")
			registration();
		if(command == "login")
			login();

    } while (command != "quit");
}

Client::~Client() { delete m_socket; }

void Client::copyRegistrationItem(const Item& item) {
	m_user.setID(item.getID());
	m_user.setNickname(item.getNick().toStdString());
}

bool Client::send(const QString &data){
	m_socket->write(data.toLatin1().data());
	if (m_socket->waitForBytesWritten(300)) {
		if(DEBUG) std::cout << "Client::send - success" << std::endl;
		return true;
	} else {
		if(DEBUG) std::cout << "Client::send - fail" << std::endl;
		return false;
	}
}

bool Client::receive(QByteArray& response) {
	if(m_socket->waitForReadyRead(1000)){
		if(DEBUG) std::cout << "\nClient::receive - available bytes on Socket: " << m_socket->bytesAvailable() << std::endl;
		response = m_socket->readAll(); //REGISTER_RESPONSE received
		return true;
	} else {
		if(DEBUG) std::cout << "\nClient::receive - No data received" << std::endl;
		return false;
	}
}