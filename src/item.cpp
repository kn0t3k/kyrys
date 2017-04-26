#include <reference.hpp>
#include <item.hpp>
#include <serverResolver.hpp>
#include <utils.hpp>

#define DEBUG 1

#define FORMAT_SEPARATOR ";"
#define FORMAT_NEW_LINE "\r\n"

using Kyrys::Item;

using Kyrys::ServerResolver;
using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Utils::Random;

//Constructors
Item::Item(const QJsonObject &json)
        : m_name(),
          m_nick(),
          m_nickOriginal(),
          m_passwordHash(),
          m_forwardTo(),
          m_args(){
    m_methodType = MethodType::UNKNOWN;
    m_extension = 0;
    m_ID = 0;
	m_nick_modified = false;
	m_success = 0;

	if(DEBUG)std::cout << "\nItem::parametric_constructor called" << std::endl;

	parse(json); //todo: change this according to Issue #27.16
}

Item::Item()
        : m_name(),
          m_nick(),
          m_nickOriginal(),
          m_passwordHash(),
          m_forwardTo(),
          m_args(){
    m_methodType = MethodType::UNKNOWN;
    m_extension = 0;
    m_ID = 0;
	m_nick_modified = false;
	m_success = 0;

	if(DEBUG)std::cout << "\nItem::empty_constructor called" << std::endl;
}


//Getters
const MethodType &Kyrys::Item::getMethodType() const { return m_methodType; }
const QString &Kyrys::Item::getName() const { return m_name; }
const QString &Kyrys::Item::getNick() const { return m_nick; }
int Item::getID() const { return m_ID; }
const QString &Item::getPasswordHash() const { return m_passwordHash; }
bool Item::getNick_modified() const { return m_nick_modified; }
bool Item::getSuccess() const { return m_success; }
const QString &Item::getRecepient() const { return m_forwardTo; }
const QString &Item::getArgs() const { return m_args; }

//Setters
void Item::setID(int ID) { m_ID = ID; }


//Other methods
std::string Item::serialize(int ID) const {
    std::string output = std::to_string(ID);

    output += FORMAT_SEPARATOR;
    output += m_name.toStdString();
    output += FORMAT_SEPARATOR;
    output += m_nick.toStdString();
    output += FORMAT_SEPARATOR;
    output += m_passwordHash.toStdString();
    output += FORMAT_NEW_LINE;
    return output;
}


//todo: refactor and expand isValid system
//Validation system
int Item::isValid() const {
    if (m_methodType == MethodType::INVALID_CMND)
        return Status::INVALID_CMND;
    if (m_methodType == MethodType::UNKNOWN)
        return Status::UNKNOWN_METHOD;

    if (m_methodType == MethodType::REGISTER) {
        if (m_nick.isEmpty() || m_passwordHash.isEmpty()) {
            return Status::INVALID_CMND;
        }
    }

    if (m_methodType == MethodType::LOGIN) {
        if (m_nick.isEmpty() || m_passwordHash.isEmpty()) {
            return Status::INVALID_CMND;
        }
    }

    if (m_methodType == MethodType::FORWARD) {
        if (m_args.isEmpty()) {
            return Status::INVALID_CMND;
        }
    }
    return Status::SUCCESS;
}


//Parsing system
void Item::parse(const QJsonObject &json) {

	if(DEBUG)std::cout << "\nItem::parse called" << std::endl;

	//Checking if method is invalid
	if (json["method"].toString().isEmpty()) {
        m_methodType = MethodType::INVALID_CMND;
		return;
	}

	//Checking if method = register
	if (json["method"].toString() == "register") {
		if(DEBUG)std::cout << "\nItem::parse -> register" << std::endl;
		if(json["messageType"].toString() == "REGISTER_REQUEST") {
			if(DEBUG)std::cout << "\nREGISTER_REQUEST" << std::endl;
			parseRegisterRequest(json);
			return;
		}
		if(json["messageType"].toString() == "REGISTER_RESPONSE") {
			if(DEBUG)std::cout << "\nREGISTER_RESPONSE" << std::endl;
			parseRegisterResponse(json);
			return;
		}
	}

	//Checking if method = login
	if (json["method"].toString() == "login") {
		if(json["messageType"].toString() == "LOGIN_REQUEST") {
			parseLoginRequest(json);
			return;
		}
		if(json["messageType"].toString() == "LOGIN_RESPONSE") {
			parseLoginResponse(json);
			return;
		}
	}

	//Checking if method = forward
	if (json["method"].toString() == "forward") {
		parseForward(json);
		return;
	}

	/*
	 * +-----------------------------------------------------------------------------------+
	 * |  Add new parsing method here like it is described in doxygen comment in Item.hpp  |
	 * +-----------------------------------------------------------------------------------+
	 */

	m_methodType = MethodType::UNKNOWN;
}

void Item::parseRegisterRequest(const QJsonObject &json){
	QJsonObject args = json["args"].toObject();

	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::REGISTER;
	}

	m_name = args["name"].toString(); //there is no "name" key in any register message
	m_nick = args["nickname"].toString(); //same as "name", this should be "nickname"
	m_passwordHash = args["password"].toString();

	m_nickOriginal = m_nick;
}

void Item::parseRegisterResponse(const QJsonObject &json){
	QJsonObject args = json["args"].toObject();
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::REGISTER;
	}
	m_nick = args["nickname"].toString();
	m_ID = args["ID"].toInt();
	//m_nick_modified = args["modified_nickname"].toBool(); //repair message first
	m_success = args["success"].toInt();

}

void Item::parseLoginRequest(const QJsonObject &json) {
	QJsonObject args = json["args"].toObject();
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::LOGIN;
	}
	m_nick = args["nickname"].toString();
	m_passwordHash = args["password"].toString();
}

void Item::parseLoginResponse(const QJsonObject &json){
	QJsonObject args = json["args"].toObject();
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::LOGIN;
	}
	std::cout << "parsing success flag" << args["success"].toInt() << std::endl; //debug
	std::cout << "parsing success flag" << args["success"].toBool() << std::endl; //debug
	m_success = args["success"].toInt();
}


void Item::parseForward(const QJsonObject &json){
	m_forwardTo = json["to"].toString();
	if (m_forwardTo.isEmpty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_args = QString(QJsonDocument(json["args"].toObject()).toJson());
		m_methodType = MethodType::FORWARD;
		qDebug() << m_args << "::" << m_forwardTo;
	}
}


//Other methods
void Item::increaseNick() {
	m_nick = m_nickOriginal + QString::number(m_extension);
	m_extension++;
}
