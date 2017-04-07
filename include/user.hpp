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
		/*todo
		 * pridaj atributy: ID uzivatela take ake ma pridelene v databaze serveru,
		 * 					sucasna IP adresa clienta
		 * 					pouzivana hashovacia metoda (neviem este ci si ju bude treba poznamenat)
		 */

	public:
		User() = default;
		User(const std::string &m_nickname, const QByteArray &m_passwordHash);
		const std::string &getNickname() const;
		const QByteArray &getPasswordHash() const;
		void setNickname(const std::string &m_nickname);
		void setPasswordHash(const QByteArray &m_passwordHash);
	};
}