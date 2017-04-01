#pragma once
#include <reference.hpp>

namespace Kyrys {

	/**
	 * @brief	class User represents one registrating, logging in or logged in user in client
	 * @note 	I expect that this class will be modified in future
	 */
	class User {
		std::string m_nickname;
		QByteArray  m_passwordHash;

	public:
		User() = default;
		User(const std::string &m_nickname, const QByteArray &m_passwordHash);
		const std::string &getNickname() const;
		const QByteArray &getPasswordHash() const;
		void setNickname(const std::string &m_nickname);
		void setPasswordHash(const QByteArray &m_passwordHash);
	};

	class Client : public QObject {
		Q_OBJECT
	public:
		explicit Client(const QString &hostName, unsigned port, QObject *parent = 0);

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
		 * @brief  This method creates json message, which send client to server when want to register new user
		 * @return
		 */
		//std::string jsonMessageRegisterCall();


		/**
		 * @brief This method creates json message, which send client to server when user want to login in
		 */
		//std::string jsonMessageLoginCall()

	private:
		QTcpSocket mSocket;
		User m_user;
	};
}
