#include <reference.hpp>
#include <user.hpp>

using Kyrys::User;

//Constructors
User::User(const std::string &nickname, const QByteArray &passwordHash)
        : m_nickname(nickname),
          m_passwordHash(passwordHash) {}

//Getters
const std::string &User::getNickname() const { return m_nickname; }

const QByteArray &User::getPasswordHash() const { return m_passwordHash; }

unsigned int User::getID() const { return m_ID; }

//Setters
void User::setNickname(const std::string &m_nickname) { User::m_nickname = m_nickname; }

void User::setPasswordHash(const QByteArray &m_passwordHash) { User::m_passwordHash = m_passwordHash; }

void User::setID(unsigned int m_ID) { User::m_ID = m_ID; }

void User::clear() {
    m_ID = 0;
    m_nickname = "";
    m_passwordHash = "";
    m_userNetworkOptions = UserNetworkOptions();
}

//Other methods
void User::printUser() {
    std::cout << "Informations about new user"
            "\nNickname: " << getNickname() <<
              "\nID: " << getID() << std::endl;
}


