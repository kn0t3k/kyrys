#include <reference.hpp>
#include <client.hpp>

#include <iostream>

int main(int argc, char **argv) {
	int portNo = Kyrys::Enums::Defaults::DefaultPortNumber::DEFAULT;
	QString hostName = "127.0.0.1";
	QCoreApplication a(argc, argv);

	QCommandLineParser parser;
	parser.setApplicationDescription("Test helper");
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addOptions({
		{
			"h",
			QCoreApplication::translate("main", "set hostname."),
			QCoreApplication::translate("main", "server hostname")
		},
		{
			"p",
			QCoreApplication::translate("main", "Server port"),
			QCoreApplication::translate("main", "port number")
		},
	});

	parser.process(a);

	if (parser.isSet("p")) {
		portNo = parser.value("p").toInt();
	}
	Kyrys::Client client(hostName, portNo, &a);
	if (!client.secureConnect()) {
		qDebug() << "client could not connect to server: " << hostName << ":" << portNo;
	} else {
		for (int i = 0 ; i < 5; i++) {
			client.sendData("ahoj!");
		}
	}

	return a.exec();
}
