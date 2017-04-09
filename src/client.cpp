#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QJsonDocument>

using Kyrys::Client;
using Kyrys::Enums::JsonMessage::MessageType;

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
	std::cout << "Choose nickname: ";
	std::getline(in, nickname);

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
			password = passwordBuffer1;
			passwordBuffer1.clear(); //security precautions
			passwordBuffer2.clear();
			return 0;
		}
	}
	return 1;
}

int Client::registration(std::istream &in) {
	using Kyrys::Enums::Client::Registration::Status;
	Status status = Status::REGISTRATION_STARTED;
	QCryptographicHash::Algorithm usedHashAlgorithm = QCryptographicHash::Sha3_512;

	std::cout << "Now follows registration procedure" << std::endl;

	std::string nickname;
	std::string password;

	if (!loadRegistrationCredentials(nickname, password, in)) {
		std::cout << "Registration failed. Check help page and try it again" << std::endl;
		return status = Status::BAD_PASSWORD;
	} else {
		status = Status::CREDENTIALS_LOADED;
	}

	const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
	m_user = User(nickname, hashed_password, usedHashAlgorithm);
	status = Status::PASSWORD_HASHED;

	QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::REGISTER_REQUEST); //this is message for server

	//todo: call server with json message and end with 0 after server answers that registration is succesfully finished on his side
	//I expect something like jsonMessageUserAuthentication -> Socket -> Server
	//Server -> Socket -> jsonMessageRegisterResponse(OK / not OK)

	return status = Status::SUCCESS;
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

	for (int i = 0; i < 5; ++i) {
		loadLoginCredentials(nickname, password, in);
		status = Status::CREDENTIALS_LOADED;

		const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), usedHashAlgorithm);
		m_user = User(nickname, hashed_password, usedHashAlgorithm);
		status = Status::PASSWORD_HASHED;

		QJsonDocument jsonMessage = jsonMessageUserAuthentication(MessageType::LOGIN_REQUEST); //this is message for server

		//TODO - contact server with JSON message obtaining hash of logging in user
	}

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
	} // missing else //what you mean? Look 3 lines upper

	args_obj["nickname"] = QString::fromStdString(m_user.getNickname());
	args_obj["password"] = m_user.getPasswordHash().toStdString().c_str();
	args_obj["hash algorithm"] = m_user.getUsedHashAlgorithm();

	root_obj.insert("args", args_obj);

	// todo add - salt of random values at the end of JSON message
	//		    - ID of message (maybe)

	return QJsonDocument(root_obj); //niesom si isty co to urobi s tym zanorenym Qjsonobject
									//snad to rekursivne prechadza celu strukturu
}


