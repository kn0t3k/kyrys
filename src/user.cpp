#include <reference.hpp>
#include <user.hpp>

using Kyrys::User;

//User class: Constructors/Getters/Setters
User::User(const std::string &m_nickname, const QByteArray &m_passwordHash) : m_nickname(m_nickname), m_passwordHash(m_passwordHash) {}

const std::string &User::getNickname() const 				 { return m_nickname; }

const QByteArray &User::getPasswordHash() const 			 { return m_passwordHash; }

void User::setNickname(const std::string &m_nickname) 		 { User::m_nickname = m_nickname; }

void User::setPasswordHash(const QByteArray &m_passwordHash) { User::m_passwordHash = m_passwordHash; }
