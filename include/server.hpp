#pragma once
#include <reference.hpp>

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

namespace Kyrys {
class Server : public QTcpServer {
	Q_OBJECT
public:
	/**
	 * @brief Construct a TCP server.
	 * @param parent QT parent object.
	 */
	explicit Server(QObject *parent = 0);

	/**
	 * @brief Starts listening on an availible port.
	 * @details This function searches for the closest availible port to the specified one
	 * by adding +1 to the port number if the supplied port is not availible.
	 * The first port that is found availible is used.
	 *
	 * @param port_no Port number.
	 */
	void startServer(qint16 port_no);

protected:
	void incomingConnection( int descriptor );
};
}
