#pragma once
#include <reference.hpp>
#include <QtCore/QCryptographicHash>

namespace Kyrys {
	/**
	 * @brief	class User represents one registrating, logging in or logged in user in client
	 * @param	m_nickname
	 * 			m_passwordHash
	 * 			m_usedHashAlgorithm - there is set which algorithm was used for hashing password
	 * @note 	I expect that this class will be modified in future
	 *
	 */
	class User {
		std::string m_nickname;
		QByteArray  m_passwordHash;
		QCryptographicHash::Algorithm m_usedHashAlgorithm;

		/*todo
		 * pridaj atributy: ID uzivatela take ake ma pridelene v databaze serveru,
		 * 					sucasna IP adresa clienta
		 * 					pouzivana hashovacia metoda (neviem este ci si ju bude treba poznamenat)
		 * 					typ socketu aky bude pouzivat user pocas komunikacie
		 */

	public:
		User() = default;
		User(const std::string &nickname, const QByteArray &passwordHash,
			 QCryptographicHash::Algorithm usedHashAlgorithm);
		const std::string &getNickname() const;
		const QByteArray &getPasswordHash() const;

		QCryptographicHash::Algorithm getUsedHashAlgorithm() const;

		void setNickname(const std::string &m_nickname);
		void setPasswordHash(const QByteArray &m_passwordHash);
	};
}