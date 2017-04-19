
#include <reference.hpp>
#include <userNetworkOptions.hpp>

//Getters
const QHostAddress &Kyrys::UserNetworkOptions::getUserIPAdress() const 			{ return m_userIPAddress; }
quint16 Kyrys::UserNetworkOptions::getPort() const 								{ return m_port; }

//Setters
void Kyrys::UserNetworkOptions::setUserIPAdress(const QHostAddress &m_ipAdress) { UserNetworkOptions::m_userIPAddress = m_ipAdress; }
void Kyrys::UserNetworkOptions::setPort(quint16 m_port) 						{ UserNetworkOptions::m_port = m_port; }
