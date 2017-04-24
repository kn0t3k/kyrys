#include <reference.hpp>
#include <user.hpp>

using Kyrys::User;

//Constructors
User::User(const std::string &nickname, const QByteArray &passwordHash, QCryptographicHash::Algorithm usedHashAlgorithm)
        : m_nickname(nickname),
          m_passwordHash(passwordHash),
          m_usedHashAlgorithm(usedHashAlgorithm) {}

//Getters
const std::string 				&User::getNickname() const 			{ return m_nickname; }
const QByteArray 				&User::getPasswordHash() const 		{ return m_passwordHash; }
QCryptographicHash::Algorithm 	 User::getUsedHashAlgorithm() const { return m_usedHashAlgorithm; }

//Setters
void User::setNickname(const std::string &m_nickname) { User::m_nickname = m_nickname; }
void User::setPasswordHash(const QByteArray &m_passwordHash) { User::m_passwordHash = m_passwordHash; }

void User::setID(unsigned int m_ID) {
	User::m_ID = m_ID;
}

void User::printUser(){
	std::cout << "Informations about new user"
			     "\nNickname: " << getNickname() <<
				 "\nID: " << getID() << std::endl;
}

unsigned int User::getID() const {
	return m_ID;
}
