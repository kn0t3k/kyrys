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
		unsigned int 					m_ID;
		std::string 					m_nickname;
		QByteArray  					m_passwordHash;
		QCryptographicHash::Algorithm 	m_usedHashAlgorithm;

	public:
		//Constructors
		User() = default;
		User(const std::string &nickname, const QByteArray &passwordHash, QCryptographicHash::Algorithm usedHashAlgorithm);

		//Getters
		const std::string &getNickname() const;
		const QByteArray &getPasswordHash() const;
		QCryptographicHash::Algorithm getUsedHashAlgorithm() const;

		//Setters
		void setNickname(const std::string &m_nickname);
		void setPasswordHash(const QByteArray &m_passwordHash);
	};
}