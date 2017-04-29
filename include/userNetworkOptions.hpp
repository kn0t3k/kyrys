#pragma once
#include <reference.hpp>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>

namespace Kyrys {
	/**
	 * @brief This class encapsulates user network properties like IP address, port, socket, etc.
	 */
	class UserNetworkOptions {
	private:
		QHostAddress 		m_userIPAddress;
		QHostAddress		m_serverIPAddress;
		quint16 			m_port; 			//server m_port

	public:

		//UserNetworkOptions();

		//Getters
		const QHostAddress &getUserIPAdress() const;
		quint16 getPort() const;

		//Setters
		void setUserIPAdress(const QHostAddress &m_ipAdress);
		void setPort(quint16 m_port);
	};
}