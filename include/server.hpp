#pragma once
#include <resolver.hpp>

#include <reference.hpp>

namespace Kyrys {
class Server : public QObject {
	Q_OBJECT
public:
	explicit Server(unsigned port, QObject *parent = 0);

	/**
	 EXAMPLE:
	 {
	 	"messageType": "REGISTER_RESPONSE",
	 	"method": "register",
	 	"args" : {
	 		"ID": "Generated ID from server's database"
	 		"nickname": {
				"modified": "1" or "0",						//Value is set to 1 in case that server modified nickname, 0 otherwise
				"value": "nickname of user"
			}
			"Status": "Success" or "fail", ...				//Status if server registered user successfully or not
		}
	}
	 */
	//std::string jsonMessageRegisterResponse(MessageType messageType);

	/**
	 * EXAMPLE:
	{
	 	"messageType": "LOGIN_RESPONSE",
	 	"method": "login",
	 	"args" : {
			"Status": "Success" or "fail", ...		//Status if user logged in successfully or not
		}
	}
	 */
	//std::string jsonMessageLoginResponse(MessageType messageType);

signals:

public slots:
	void newConnection();

private:
	QTcpServer *m_server;
	Resolver m_resolver;
};
}
