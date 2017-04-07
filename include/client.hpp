#pragma once
#include <reference.hpp>
#include <user.hpp>

namespace Kyrys {
	//typedef Kyrys::User User; //toto nepouzivaj globalne, trvalo mi pol dna zistit preco mam zablokovanu triedu User
	class Client : public QObject {
	Q_OBJECT //co je toto?
	private:
		QTcpSocket mSocket;
		User m_user;
	public:
		explicit Client(const QString &hostName, unsigned port, QObject *parent = 0);

		int loadRegistrationCredentials(std::string &nickname, std::string &password);

		int loadLoginCredentials(std::string &nickname, std::string &password);

		/**
		 * @brief 	 Registration of new user
		 * @warning  User's password has to be salted and hashed inside method. Password must not leave method in open format.
		 * @return	 0 in case of succesfull registration, non-zero code otherwise
		 * 			 code 1 = user was not able to type same password twice at least one time
		 */
		int registration(); //not finished yet

		/**
		 * @brief  Login of registered user
		 * @return Same as registration
		 */
		int login(); //not finished yet


		/**
		 * @brief  This method creates json message, which send client to server when want to authenticate user. It is used in both cases, login and registration
		 * @param messageType	write MessageType::LOGIN_CALL in case of login
		 * 							  MessageType::REGISTER_CALL otherwise
		 * @return message in format QJsonDocument
		 * @note   message format:
		 * 		   {
    				"method": "register/call",
    				"nickname": "some nick",
    				"password": "hash of password"
				   }
		 */
		QJsonDocument jsonMessageUserAuthentication(Kyrys::Enums::JsonMessage::MessageType messageType);
	};
}
