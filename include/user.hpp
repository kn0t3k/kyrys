#pragma once

#include <reference.hpp>
#include <QtCore/QCryptographicHash>
#include <userNetworkOptions.hpp>

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
        unsigned int m_ID;
        std::string m_nickname;
        QByteArray m_passwordHash;
        UserNetworkOptions m_userNetworkOptions;

        //TODO: Add list of friends(HASH TABLE) and maybe more member atributes
        //add userHistory

    public:
        //Constructors
        User() = default;

        User(const std::string &nickname, const QByteArray &passwordHash);

        //Getters
        const std::string &getNickname() const;

        const QByteArray &getPasswordHash() const;

        unsigned int getID() const;

        //Setters
        void setNickname(const std::string &m_nickname);

        void setPasswordHash(const QByteArray &m_passwordHash);

        void setID(unsigned int m_ID);

        void clear();

        //Printers
        void printUser();
    };
}