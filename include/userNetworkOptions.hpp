#pragma once
#include <reference.hpp>
#include <QtNetwork/QAbstractSocket>

namespace Kyrys {

	typedef Kyrys::Enums::User::SocketType SocketType;

	/**
	 * @brief This class encapsulates user network properties like IP address, port, socket, etc.
	 */
	class UserNetworkOptions {
	private:
		QHostAddress 		m_ipAddress;
		quint16 			m_port;
		SocketType	 		m_socketType;
		QAbstractSocket 	m_socket;

	public:
		//Getters
		const QHostAddress &getIPAdress() const;
		quint16 getPort() const;
		SocketType getSocketType() const;
		const QAbstractSocket &getSocket() const;

		//Setters
		void setIPAdress(const QHostAddress &m_ipAdress);
		void setPort(quint16 m_port);
		void setSocketType(SocketType m_socketType);
		//void setSocket(const QAbstractSocket &m_socket);
	};
}