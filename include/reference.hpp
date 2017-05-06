#pragma once

//Qt Network
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QSslError>

//Qt JSON
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

//Qt basic data types ?
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QObject>
#include <QtCore/QJsonValue>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDataStream>
#include <QtCore/QCommandLineParser>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QList>
#include <QtCore/QDir>
#include <QtCore/QBuffer>

#include <QtConcurrent/QtConcurrent>

//Qt SQL
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

//STD library
#include <iostream>
#include <fstream>
#include <limits>

//Our own *.hpp files from include directory
#include <enums.hpp>

//mbedTLS Library: files needed for entropy, random numbers generator, Diffie-Hellman algorithm and AES cipher
#include "mbedtls/aes.h"


typedef std::string string;
