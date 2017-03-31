#pragma once
#include <reference.hpp>

namespace Kyrys {
class Client : public QObject {
	Q_OBJECT
public:
	explicit Client(const QString &hostName, unsigned port, QObject *parent = 0);
	
private:
	QTcpSocket mSocket;
};
}
