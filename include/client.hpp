#pragma once
#include <reference.hpp>
#include <user.hpp>
#include <istream>

namespace Kyrys {
class Client : public QObject {
	typedef Kyrys::User User;
	typedef std::string string;
	typedef Kyrys::Enums::JsonMessage::MessageType MessageType;

	Q_OBJECT 
private:
	QTcpSocket m_socket;
	User m_user;
public:
	explicit Client(const QString &hostName, unsigned port, QObject *parent = 0);

	const User &getUser() const;

	int loadRegistrationCredentials(string &nickname, string &password, std::istream &in = std::cin);

	int loadLoginCredentials(string &nickname, string &password, std::istream &in = std::cin);

	/**
	 * @brief 	 Registration of new user
	 * @warning  User's password has to be salted and hashed inside method. Password must not leave method in open format.
	 * @return	 0 in case of succesfull registration, non-zero code otherwise
	 * 			 code 1 = user was not able to type same password twice at least one time
	 */
	int registration(std::istream &in = std::cin); //not finished yet

	/**
	 * @brief  Login of registered user
	 * @return Same as registration
	 */
	int login(std::istream &in = std::cin); //not finished yet


	/**
	 * @brief  This method creates json message, which send client to server when want to authenticate user. It is used in both cases, login and registration
	 * @param messageType	write MessageType::LOGIN_CALL in case of login
	 * 							  MessageType::REGISTER_CALL otherwise
	 * @return message in format QJsonDocument
	 * @note   message format examples:
	 * 		   EXAMPLE 1:
	 * 		   {
	 * 		   		"messageType": "REGISTER_REQUEST",
					"method": "register",
					"args" : {
						"nickname": "nickname of user",
						"password": "hash of user's password",
						"hash algorithm": "sha3_512",
			   		}
			   }

			   EXAMPLE 2:
			   {
	 		   		"messageType": "LOGIN_REQUEST",
					"method": "login",
					"args" : {
						"nickname": "nickname of user",
						"password": "hash of user's password"
						"hash algorithm": "sha3_512",
			   		}
			   }


	 */
	QJsonDocument jsonMessageUserAuthentication(MessageType messageType);
};
}
