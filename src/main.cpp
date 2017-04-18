#include <reference.hpp>
#include <server.hpp>
#include <client.hpp>

int main(int argc, char **argv) {
	unsigned defaultPort = 12345;
	QCoreApplication a(argc, argv);

	QCommandLineParser parser;
	parser.setApplicationDescription("Test helper");
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addOptions({
		{
			"c",
			QCoreApplication::translate("main", "Run as client."),
			QCoreApplication::translate("main", "server name")
		},
		{
			"s",
			QCoreApplication::translate("main", "Run as server.")
		},
		{
			"p",
			QCoreApplication::translate("main", "Server port"),
			QCoreApplication::translate("main", "port number")
		},
	});

	parser.process(a);

	if (parser.isSet("s")) {
		qDebug() << "running server";
		if (parser.isSet("p")) {
			Kyrys::Server server(parser.value("p").toInt());
			return a.exec();
		} else {
			Kyrys::Server server(defaultPort);
			return a.exec();
		}
	} else if (parser.isSet("c")) {
		if (parser.isSet("p")) {
			Kyrys::Client client("localhost", parser.value("p").toUShort());
			return a.exec();
		} else {
			Kyrys::Client client("localhost", defaultPort);
			client.run();
			return a.exec();
		}
	} else {
		return 0;
	}
}
