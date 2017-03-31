#pragma once
#include <resolver.hpp>

#include <reference.hpp>

namespace Kyrys {
class Server : public QObject {
	Q_OBJECT
public:
	explicit Server(unsigned port, QObject *parent = 0);

signals:

public slots:
	void newConnection();

private:
	QTcpServer *mServer;
	Resolver mResolver;
};
}
