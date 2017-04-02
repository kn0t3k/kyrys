#include <reference.hpp>
#include <client.hpp>
#include <QtCore/QCryptographicHash>

using Kyrys::Client;

Client::Client(const QString &hostName, unsigned port, QObject *parent) : QObject(parent), m_user() {
	QString data;
	mSocket.connectToHost(hostName, port); //warning: connectToHost expects port as quint16 type

	QByteArray buffer;
	buffer.append(mSocket.readAll());	

	qDebug() << buffer;
}

int Client::registration(){
	bool success = false; //dokoncit if
	std::cout << "Now follows registration procedure"
				 "Choose nickname: ";
	std::string nickname;
	std::getline(std::cin, nickname);

	//todo: client should ask server if nickname is already used. If yes, then user have to choose another nickname

	std::string password, password2;
	for(int i = 0; i < 5; ++i){ //User has 5 tries to type password correctly twice in a row
		std::cout << "\nChoose password: ";
		std::getline(std::cin, password);
		std::cout << "\nRepeat the password:";
		std::getline(std::cin, password2);
		if(password != password2) {
			std::cout << "Passwords are not same!, please try it again" << std::endl;
			password.clear();
			password2.clear();
		} else
			break;
	}
	if(password != password2) {
		std::cout << "Registration failed. Check help page and try it again" << std::endl;
		return 1;
	}

	QCryptographicHash hash(QCryptographicHash::Sha3_512); //Creates QCryptographicHash object
	hash.addData(QByteArray::fromStdString(password));     //set context of QCryptographicHash
	m_user.User(nickname, hash.result());

	//todo: call server with json message and end with 0 after server answers that registration is succesfully finished on his side
	//I expect something like jsonMessageRegisterCall -> Socket -> Server
	//Server -> Socket -> jsonMessageRegisterResponse(OK / not OK)

	return 0;
}

int Client::login (){
	std::cout << "Nickname: ";
	std::string nickname;
	std::getline(std::cin, nickname);
	std::string password;

	QCryptographicHash hash(QCryptographicHash::Sha3_512);

	for(int i = 0; i < 5; ++i){
		std::cout << "\nPassword: ";
		std::getline(std::cin, password);
		hash.addData(QByteArray::fromStdString(password));
		//TODO - contact server with JSON message obtaining hash of logging in user
	}

	return 0;
}

