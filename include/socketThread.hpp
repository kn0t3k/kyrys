#pragma once
#include <reference.hpp>

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

namespace Kyrys {
class SocketThread : public QThread {
	Q_OBJECT

	int m_socketID;

	QSslSocket *m_socket = nullptr;

private slots:

	void readData();
	void quit();
	void encrypted();
	
public:
	/**
	 * @brief Construct a thread, that will represent a connection between client and server.
	 * This is a key concept since the server will be able to server multiple clients simultaneously.
	 * @param socketID Socket descriptor of the new connection.
	 * @param parent QT parent object.
	 */
	explicit SocketThread(int socketID, QObject *parent = 0);

	/**
	 * @brief Starts the execution of the newly created thread.
	 * That means communicating with the client, sending responses etc.
	 */
	void run() override;

public slots:
	void sslError( QList<QSslError> errors );

};

}