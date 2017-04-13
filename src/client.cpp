#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>

using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;

typedef Kyrys::Enums::Client::Login::Status 	   loginStatus;
typedef Kyrys::Enums::Client::Registration::Status registrationStatus;

//Constructors
Client::Client(const QString &hostName, unsigned port, QObject *parent) : QObject(parent), m_user() {
	QString data;
	m_socket.connectToHost(hostName, port); //warning: connectToHost expects port as quint16 type

	QByteArray buffer;
	buffer.append(m_socket.readAll());

	qDebug() << buffer;
}

//Getters
const Client::User &Client::getUser() const { return m_user; }

int Client::loadRegistrationCredentials(std::string &nickname, std::string &password, std::istream &in) {
	std::string nicknameBuffer;
	std::cout << "Choose nickname: ";
	std::getline(in, nicknameBuffer);

	std::string passwordBuffer1;
	std::string passwordBuffer2;

	for (int i = 0; i < 5; ++i) { //User has 5 tries to type password correctly twice in a row
		std::cout << "\nChoose password: ";
		std::getline(in, passwordBuffer1);
		std::cout << "\nRepeat the password:";
		std::getline(in, passwordBuffer2);
		if (passwordBuffer1 != passwordBuffer2) {
			std::cout << "Passwords are not same!, please try it again" << std::endl;
			passwordBuffer1.clear();
			passwordBuffer2.clear();
		} else {
			nickname = nicknameBuffer;
			password = passwordBuffer1;
			passwordBuffer1.clear(); //security precautions
			passwordBuffer2.clear();
			return registrationStatus::SUCCESS;
		}
	}
	return registrationStatus::BAD_PASSWORD;
}

int Client::registration(std::istream &in) {
	registrationStatus status = registrationStatus::REGISTRATION_STARTED;
	QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;

	std::cout << "Now follows registration procedure" << std::endl;

	std::string nickname;
	std::string password;

	if (loadRegistrationCredentials(nickname, password, in) == registrationStatus::BAD_PASSWORD) {
		std::cout << "Registration failed. Check help page and try it again" << std::endl;
		return status = registrationStatus::BAD_PASSWORD;
	} else {
		status = registrationStatus::CREDENTIALS_LOADED;
	}

	const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
	m_user = User(nickname, hashed_password, usedHashAlgorithm);
	status = registrationStatus::PASSWORD_HASHED;

	QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::REGISTER_REQUEST); //this is message for server

	//todo: call server with json message and end with 0 after server answers that registration is succesfully finished on his side
	//I expect something like jsonMessageUserAuthentication -> Socket -> Server
	//Server -> Socket -> jsonMessageRegisterResponse(OK / not OK)

	return status = registrationStatus::SUCCESS;
}

int Client::loadLoginCredentials(std::string &nickname, std::string &password, std::istream &in) {
	std::cout << "Nickname: ";
	std::getline(in, nickname);
	std::cout << "\nPassword: ";
	std::getline(in, password);
	return 0;
}

int Client::login (std::istream &in) {
	using Kyrys::Enums::Client::Login::Status;
	Status status = Status::LOGIN_STARTED;
	std::string nickname, password;
	QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;

	//cyclus begin
	loadLoginCredentials(nickname, password, in);
	status = Status::CREDENTIALS_LOADED;

	QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
	m_user = User(nickname, hashed_password, usedHashAlgorithm);
	hashed_password.clear();
	status = Status::PASSWORD_HASHED;

	QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::LOGIN_REQUEST); //this is message for server
	//cyclus end

	//TODO - contact server with JSON message obtaining hash of logging in user

	return status = Status::SUCCESS;
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


