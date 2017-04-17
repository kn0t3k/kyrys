#pragma once
#include <reference.hpp>
#include <userNetworkOptions.hpp>

const QHostAddress &Kyrys::UserNetworkOptions::getIPAdress() const {
	return m_ipAddress;
}

quint16 Kyrys::UserNetworkOptions::getPort() const {
	return m_port;
}

Kyrys::SocketType Kyrys::UserNetworkOptions::getSocketType() const {
	return m_socketType;
}

const QAbstractSocket &Kyrys::UserNetworkOptions::getSocket() const {
	return m_socket;
}

void Kyrys::UserNetworkOptions::setIPAdress(const QHostAddress &m_ipAdress) {
	UserNetworkOptions::m_ipAddress = m_ipAdress;
}

void Kyrys::UserNetworkOptions::setPort(quint16 m_port) {
	UserNetworkOptions::m_port = m_port;
}

void Kyrys::UserNetworkOptions::setSocketType(Kyrys::SocketType m_socketType) {
	UserNetworkOptions::m_socketType = m_socketType;
}
/*
void Kyrys::UserNetworkOptions::setSocket(const QAbstractSocket &m_socket) {
	UserNetworkOptions::m_socket = m_socket;
}
*/