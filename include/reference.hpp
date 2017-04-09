#pragma once

//Qt Network
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

//Qt JSON
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

//Qt basic data types ?
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDataStream>
#include <QtCore/QCommandLineParser>
#include <QtCore/QTime>

//STD library
#include <iostream>
#include <fstream>

//Our own *.hpp files from include directory
#include <enums.hpp>

//todo : Od vytvorenia reference.hpp sme sa uz posunuli nejakym smerom
// a tak by trebalo zvazit ci sem nepridame nejake nove subory, napr. #include <QCryptographicHash> alebo std string