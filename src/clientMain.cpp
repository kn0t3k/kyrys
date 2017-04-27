#include <reference.hpp>
#include <client.hpp>

#include <iostream>

int main(int argc, char **argv) {
    int portNo = Kyrys::Enums::Defaults::DefaultPortNumber::DEFAULT_PORT;
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
        client.run(); //runs infinite loop parsing commands
        //client.sendData("{\"method\": \"register\",\"args\": {\"name\": \"Jan Novak\",\"nick\": \"Jaak\", \"password\":\"mojetajneheslo\"}}");
        //client.sendData("{\"method\": \"login\",\"args\": {\"nick\": \"Jaak1\", \"password\":\"somepass\"}}");
        //client.sendData("{\"method\": \"forward\",\"to\": \"Jaak1\", \"args\" : {\"from\" : \"me\", \"message\" : \"ahoj, jak to jde?\"}}");
    }


    return a.exec();
}
