#include <reference.hpp>
#include <client.hpp>

using Kyrys::Client;

Client::Client(const QString &hostName, unsigned port, QObject *parent) : QObject(parent) {
	QString data;
	mSocket.connectToHost(hostName, port);

	QByteArray buffer;
	buffer.append(mSocket.readAll());	

	qDebug() << buffer;
}