#include <reference.hpp>
#include <server.hpp>

#include <iostream>

int main(int argc, char **argv) {
	int portNo = Kyrys::Enums::Defaults::DefaultPortNumber::DEFAULT;
	QCoreApplication a(argc, argv);

	QCommandLineParser parser;
	parser.setApplicationDescription("Test helper");
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addOptions({
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

	Kyrys::Server server(&a);
	server.startServer(portNo);

	return a.exec();
}
