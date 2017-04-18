#pragma once
#include <reference.hpp>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>

namespace Kyrys {

	typedef Kyrys::Enums::User::SocketType SocketType;

	/**
	 * @brief This class encapsulates user network properties like IP address, port, socket, etc.
	 */
	class UserNetworkOptions {
	private:
		QHostAddress 		m_userIPAddress;
		QHostAddress		m_serverIPAddress;
		quint16 			m_port; 			//server m_port
		SocketType	 		m_socketType; 		//asi toto odstranime
		//QSslSocket 			m_socket;

	public:

		//UserNetworkOptions();

		//Getters
		const QHostAddress &getUserIPAdress() const;
		quint16 getPort() const;
		SocketType getSocketType() const;
		//const QAbstractSocket &getSocket() const;

		//Setters
		void setUserIPAdress(const QHostAddress &m_ipAdress);
		void setPort(quint16 m_port);
		void setSocketType(SocketType m_socketType);
		//void setSocket(const QAbstractSocket &m_socket);
	};
}