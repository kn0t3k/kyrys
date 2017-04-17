#pragma once
#include <reference.hpp>
#include <user.hpp>
#include <istream>

namespace Kyrys {
class Client : public QObject {
	Q_OBJECT

	typedef Kyrys::User User;
	typedef std::string string;
	typedef Kyrys::Enums::JsonMessage::MessageType MessageType;

private:
	QTcpSocket m_socket;
	User m_user;

public:
	explicit Client(const QString &hostName, unsigned port, QObject *parent = 0);

	const User &getUser() const;

	int loadRegistrationCredentials(string &nickname, string &password, std::istream &in = std::cin);
	int loadLoginCredentials(string &nickname, string &password, std::istream &in = std::cin);
	int controlPasswordSecQuality(const string& password) const;

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
	 * 		   EXAMPLE 1 - Registration:
	 * 		   {
	 * 		   		"messageType": "REGISTER_REQUEST",
					"method": "register",
					"args" : {
						"nickname": "nickname of user",
						"password": "hash of user's password",
						"hash algorithm": "sha3_512",
			   		}
			   }

			   EXAMPLE 2 - login:
			   {
	 		   		"messageType": "LOGIN_REQUEST",
					"method": "login",
					"args" : {
						"nickname": "nickname of user",
						"password": "hash of user's password"
						"hash algorithm": "sha3_512",
						"required socket": "SSL" or "TCP" //not implemented
			   		}
			   }
		@note po registracii by bolo dobre vyzadovat login ktory posle serveru dalsie informacie
	 */
	QJsonDocument jsonMessageUserAuthentication(MessageType messageType);


	/**
	 EXAMPLE:
	{
	 	"messageType": "UPDATE_REQUEST",
	 	"method": "update",
	 	"args" : {
			"IP adress": "IPv4 adress",
			"Port": "quint16 type"
		}
	}
	 */
	//QJsonDocument jsonMessageUserUpdate(MessageType messageType);

	/**
	 * @brief run	It's a method executing commands from Client commandline.
	 * @param in 	In is parameter for pushing fake input instead of standard input from std::cin. Use this param for testing purpose. Default value is std::cin
	 */
	void run(std::istream& in = std::cin);
};
}
