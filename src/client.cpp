#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>

using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;

Client::Client(const QString &hostName, unsigned port, QObject *parent) : QObject(parent), m_user() {
	QString data;
	m_socket.connectToHost(hostName, port); //warning: connectToHost expects port as quint16 type

	QByteArray buffer;
	buffer.append(m_socket.readAll());

	qDebug() << buffer;
}

int Client::loadRegistrationCredentials(std::string& nickname, std::string& password) {
	std::cout << "Choose nickname: ";
	std::getline(std::cin, nickname);

	std::string buffer;
	for (int i = 0; i < 5; ++i) { //User has 5 tries to type password correctly twice in a row
		std::cout << "\nChoose password: ";
		std::getline(std::cin, password);
		std::cout << "\nRepeat the password:";
		std::getline(std::cin, buffer);
		if (password != buffer) {
			std::cout << "Passwords are not same!, please try it again" << std::endl;
			password.clear();
			buffer.clear();
		} else {
			buffer.clear(); //security precaution
			return 0;
		}
	}
	return 1;
}

int Client::registration() {
	using Kyrys::Enums::Client::Registration::Status;
	Status status = Status::REGISTRATION_STARTED;

	std::cout << "Now follows registration procedure" << std::endl;

	std::string nickname;
	std::string password;

	if (!loadRegistrationCredentials(nickname, password)) {
		std::cout << "Registration failed. Check help page and try it again" << std::endl;
		return status = Status::BAD_PASSWORD;
	} else {
		status = Status::CREDENTIALS_LOADED;
	}

	const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), QCryptographicHash::Sha3_512);
	m_user = User(nickname, hashed_password);
	status = Status::PASSWORD_HASHED;

	QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::REGISTER); //this is message for server

	//todo: call server with json message and end with 0 after server answers that registration is succesfully finished on his side
	//I expect something like jsonMessageUserAuthentication -> Socket -> Server
	//Server -> Socket -> jsonMessageRegisterResponse(OK / not OK)

	return status = Status::SUCCESS;
}

int Client::loadLoginCredentials(std::string &nickname, std::string &password) {
	std::cout << "Nickname: ";
	std::getline(std::cin, nickname);
	std::cout << "\nPassword: ";
	std::getline(std::cin, password);
	return 0;
}

int Client::login () {
	using Kyrys::Enums::Client::Login::Status;
	Status status = Status::LOGIN_STARTED;
	std::string nickname, password;

	for (int i = 0; i < 5; ++i) {
		loadLoginCredentials(nickname, password);
		status = Status::CREDENTIALS_LOADED;

		const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), QCryptographicHash::Sha3_512);
		m_user = User(nickname, hashed_password);
		status = Status::PASSWORD_HASHED;

		QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::LOGIN); //this is message for server

		//TODO - contact server with JSON message obtaining hash of logging in user
	}

	return status = Status::SUCCESS;
}

QJsonDocument Client::jsonMessageUserAuthentication(MessageType messageType) {
	QJsonObject jsonObject;
	QString key = "method";
	QString value;
	if (messageType == MessageType::REGISTER) {
		value = "register";
	} else {
		value = "login";
	} // missing else

	jsonObject.insert(key, value);

	key = "nickname";
	value = QString::fromStdString(m_user.getNickname());
	jsonObject.insert(key, value);

	key = "password";
	jsonObject.insert(key, m_user.getPasswordHash().toStdString().c_str());
	// todo add - salt of random values at the end of JSON message
	//		    - ID of message (maybe)

	return QJsonDocument(jsonObject);
}

