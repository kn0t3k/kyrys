#include <server.hpp>

#include <reference.hpp>

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

    Kyrys::Server server;

    return a.exec();
}
