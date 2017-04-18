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
	QTcpSocket m_socket; //This socket should serve the client not user, so it is used for registration and login
	quint16 m_port; //quint16 is actually typedef unsigned short so for conversion from qstring to quint16 use method QString::toUshort
				  //from client's socket point of view, it's probably server's incoming m_port
	User m_user; //Current user

public:
	explicit Client(const QString &hostName, quint16 port, QObject *parent = 0);

	const User &getUser() const;

	int loadRegistrationCredentials(string &nickname, string &password, std::istream &in = std::cin);

	int loadLoginCredentials(string &nickname, string &password, std::istream &in = std::cin);

	/**
	 * @brief			This method controls if password meets the safety requiremets
	 * @param password	Password whish will be controlled
	 * @return			PasswordSecQuality::GOOD = 0 if password is ok different value otherwise, more info in enums.hpp
	 */
	int controlPasswordSecQuality(const string& password) const;

	/**
	 * @brief 	 Registration of new user
	 * @warning  User's password has to be salted and hashed inside method. Password must not leave method in open format.
	 * @return	 0 in case of succesfull registration, non-zero code otherwise
	 * 			 code 1 = user was not able to type same password twice at least one time
	 * @note	 Registered user is not automatically logged in. After registration you have to pass login procedure.
	 */
	int registration(std::istream &in = std::cin); //not finished yet

	/**
	 * @brief  Login of registered user
	 * @return Same as registration
	 * @note
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
						"hash algorithm": "sha3_512", //delete hash algorithm everywhere
			   		}
			   }

			   EXAMPLE 2 - login:
			   {
	 		   		"messageType": "LOGIN_REQUEST",
					"method": "login",
					"args" : {
						"nickname": "nickname of user",
						"password": "hash of user's password"
						"hash algorithm": "sha3_512", //delete
						"required socket": "SSL" or "TCP" //delete yatial napevno sssl
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
