#include <reference.hpp>
#include <client.hpp>


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

    QThread *clientThread = new QThread;
    Kyrys::Client *client = new Kyrys::Client(hostName, static_cast<quint16>(portNo));

    client->moveToThread(clientThread);

    QObject::connect(clientThread, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(clientThread, SIGNAL(started()), client, SLOT(secureConnect()));
    QObject::connect(client, SIGNAL(clientFinished()), client, SLOT(deleteLater()));

    clientThread->start();

    return a.exec();
}
