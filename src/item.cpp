#include <reference.hpp>
#include <item.hpp>
#include <serverResolver.hpp>
#include <utils.hpp>

#define FORMAT_SEPARATOR ";"
#define FORMAT_NEW_LINE "\r\n"

using Kyrys::Item;

using Kyrys::ServerResolver;
using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::JsonMessage::MessageType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Utils::Random;

//Constructors
Item::Item() {
    clear();
    if (DEBUG)std::cout << "\nItem::empty_constructor called" << std::endl;
}

//Getters
const MethodType &Kyrys::Item::getMethodType() const { return m_methodType; }

const MessageType &Kyrys::Item::getMessageType() const { return m_messageType; }

const QString &Kyrys::Item::getNick() const { return m_nick; }

int Item::getID() const { return m_ID; }

const QString &Item::getPasswordHash() const { return m_passwordHash; }

bool Item::getNick_modified() const { return m_nick_modified; }

bool Item::getSuccess() const { return m_success; }

const QString &Item::getArgs() const { return m_args; }

unsigned int Item::getFromID() const { return m_fromID; }

unsigned int Item::getToID() const { return m_toID; }

const QString &Item::getToNick() const { return m_toNick; }

Item::Accessibility Item::getAccessibility() const { return m_accessibility; }

Item::Encryption Item::getEncryption() const { return m_encryption; }

bool Item::getAnswer() const { return m_answer; }

const QString &Item::getData() const { return m_data; }

//Setters
void Item::setID(int ID) { m_ID = ID; }

void Item::clear() {
    m_methodType = MethodType::UNKNOWN;;
    m_messageType = MessageType::UNKNOWN;
    m_nick = "";
    m_nickOriginal = "";
    m_passwordHash = "";
    m_args = "";
    m_extension = 0;
    m_ID = 0;
    m_nick_modified = false;
    m_success = 0;

	//Chatting params
	m_fromID = std::numeric_limits<unsigned int>::max();
	m_toID = std::numeric_limits<unsigned int>::max();
	m_toNick = "";
	m_accessibility = Accessibility::OFFLINE;
	m_encryption = Encryption::PLAIN_TEXT;
	m_answer = false;
	m_data = "";
}

//Validation system
int Item::isValid() const {
    //Validating of Unknown or invalid messages
    if (m_methodType == MethodType::INVALID_CMND)
        return Status::INVALID_CMND;
    if (m_methodType == MethodType::UNKNOWN)
        return Status::UNKNOWN_METHOD;

    //Validating of Registration messages
    if (m_methodType == MethodType::REGISTER) {
        if (m_messageType == MessageType::REGISTER_REQUEST)
            return isValidRegisterRequest();
        if (m_messageType == MessageType::REGISTER_RESPONSE)
            return isValidRegisterResponse();
    }

    //Validating of Login messages
    if (m_methodType == MethodType::LOGIN) {
        if (m_messageType == MessageType::LOGIN_REQUEST)
            return isValidLoginRequest();
        if (m_messageType == MessageType::LOGIN_RESPONSE)
            return isValidLoginResponse();
    }

    //Validation of Chat messages
    if (m_methodType == MethodType::CHAT){
        if(m_messageType == MessageType::CHAT_REQUEST)
            return isValidChatRequest();
        if(m_messageType == MessageType::CHAT_RESPONSE)
            return isValidChatResponse();
        if(m_messageType == MessageType::CHAT_DATA)
            return isValidChatData();
    }

    return Status::UNKNOWN_METHOD;
}


//Validating methods
int Item::isValidRegisterRequest() const {
    if (m_nick.isEmpty() || m_passwordHash.isEmpty())
        return Status::INVALID_CMND;
    return Status::SUCCESS;
}

int Item::isValidRegisterResponse() const {//todo
    if (m_ID < 0)
        return Status::INVALID_CMND;
    return Status::SUCCESS;
}

int Item::isValidLoginRequest() const {
    if (m_nick.isEmpty() || m_passwordHash.isEmpty())
        return Status::INVALID_CMND;
    return Status::SUCCESS;
}

int Item::isValidLoginResponse() const {
    return Status::SUCCESS;
}

int Item::isValidChatSourceDest() const{
    if(m_fromID != std::numeric_limits<unsigned int>::max() && (m_toID != std::numeric_limits<unsigned int>::max() || !m_toNick.isEmpty()))
		return Status::SUCCESS;
	else
		return Status::INVALID_JSON;
}

int Item::isValidChatRequest() const{
	if(m_encryption != Encryption::PLAIN_TEXT && m_encryption != Encryption::SHARED_KEY)
		return Status::INVALID_JSON;
	else
		return isValidChatSourceDest();
}

int Item::isValidChatResponse() const{
    if(m_accessibility > Accessibility::CHATTING)
		return Status::INVALID_JSON;
    else return isValidChatSourceDest();
}

int Item::isValidChatData() const{
    return isValidChatSourceDest();
}


//Parsing system
void Item::parse(const QJsonObject &json) {

    //Checking if method is invalid
    if (json["method"].toString().isEmpty()) {
		if(DEBUG_BIN)std::cout << "\nItem::parse : error - JSON message has empty method\n" << std::endl;
		m_methodType = MethodType::INVALID_CMND;
		return;
	}

    //Checking if method = register
    if (json["method"].toString() == "register") {
        if (json["messageType"].toString() == "REGISTER_REQUEST") {
            parseRegisterRequest(json);
            return;
        }
        if (json["messageType"].toString() == "REGISTER_RESPONSE") {
            parseRegisterResponse(json);
            return;
        }
    }

    //Checking if method = login
    if (json["method"].toString() == "login") {
        if (json["messageType"].toString() == "LOGIN_REQUEST") {
            parseLoginRequest(json);
            return;
        }
        if (json["messageType"].toString() == "LOGIN_RESPONSE") {
            parseLoginResponse(json);
            return;
        }
    }

    //Checking if method = chat
    if(json["method"].toString() == "chat"){
		// todo: ulozit kopii zpravy do QDocument
        if(json["messageType"].toString() == "CHAT_REQUEST"){
            parseChatRequest(json);
            return;
        }
        if(json["messageType"].toString() == "CHAT_RESPONSE"){
            parseChatResponse(json);
            return;
        }
        if(json["messageType"].toString() == "CHAT_DATA"){
            parseChatData(json);
            return;
        }
    }

    /*
     * +-----------------------------------------------------------------------------------+
     * |  Add new parsing method here like it is described in doxygen comment in Item.hpp  |
     * +-----------------------------------------------------------------------------------+
     */

	if(DEBUG_BIN)std::cout << "\nItem::parse : error - JSON message has UNKNOWN method\n" << std::endl;
    m_methodType = MethodType::UNKNOWN;
}


void Item::parseRegisterRequest(const QJsonObject &json) {
    m_messageType = MessageType::REGISTER_REQUEST;

    QJsonObject args = json["args"].toObject();

    if (args.empty()) {
        m_methodType = MethodType::INVALID_CMND;
    } else {
        m_methodType = MethodType::REGISTER;
    }

    m_nick = args["nickname"].toString();
    m_passwordHash = args["password"].toString();

    m_nickOriginal = m_nick;
}


void Item::parseRegisterResponse(const QJsonObject &json) {
    m_messageType = MessageType::REGISTER_RESPONSE;

    QJsonObject args = json["args"].toObject();
    if (args.empty()) {
        m_methodType = MethodType::INVALID_CMND;
    } else {
        m_methodType = MethodType::REGISTER;
    }
    m_nick = args["nickname"].toString();
    m_ID = args["ID"].toInt();
    //m_nick_modified = args["modified_nickname"].toBool(); //repair message first

	if(args["success"].toInt() == 1)
		m_success = true;
	else
		m_success = false;

}


void Item::parseLoginRequest(const QJsonObject &json) {
    m_messageType = MessageType::LOGIN_REQUEST;

    QJsonObject args = json["args"].toObject();
    if (args.empty()) {
        m_methodType = MethodType::INVALID_CMND;
    } else {
        m_methodType = MethodType::LOGIN;
    }
    m_nick = args["nickname"].toString();
    m_passwordHash = args["password"].toString();
}


void Item::parseLoginResponse(const QJsonObject &json) {
    m_messageType = MessageType::LOGIN_RESPONSE;

    QJsonObject args = json["args"].toObject();
    if (args.empty()) {
        m_methodType = MethodType::INVALID_CMND;
    } else {
        m_methodType = MethodType::LOGIN;
    }
    if(DEBUG)std::cout << "parsing success flag" << args["success"].toInt() << std::endl; //debug
    if(DEBUG)std::cout << "parsing success flag" << args["success"].toBool() << std::endl; //debug
    m_success = args["success"].toInt();
}


void Item::parseChatSourceDest(const QJsonObject &json){
	QJsonObject args = json["args"].toObject();
    m_args = QString(QJsonDocument(args).toJson()); //remove this line if you don't need it in serverResolver
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_fromID = args["fromID"].toInt();
		m_toID = args["toID"].toInt();
	}
}


void Item::parseChatRequest(const QJsonObject &json){
	m_messageType = MessageType::CHAT_REQUEST;

	QJsonObject args = json["args"].toObject();
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::CHAT;
		parseChatSourceDest(json);
		m_toNick = args["toNick"].toString();
		m_encryption = static_cast<Encryption>(args["dataEncryption"].toString().toUInt());
	}
}


void Item::parseChatResponse(const QJsonObject &json){
	m_messageType = MessageType::CHAT_RESPONSE;

	QJsonObject args = json["args"].toObject();
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::CHAT;
		parseChatSourceDest(json);
		m_accessibility = static_cast<Accessibility>(args["accessibility"].toString().toUInt());
		m_answer = args["answer"].toBool();
	}
}


void Item::parseChatData(const QJsonObject &json){
	m_messageType = MessageType::CHAT_DATA;

	QJsonObject args = json["args"].toObject();
	if (args.empty()) {
		m_methodType = MethodType::INVALID_CMND;
	} else {
		m_methodType = MethodType::CHAT;
		parseChatSourceDest(json);
		m_data = args["data"].toString();
	}
}


//Other methods
void Item::increaseNick() {
    m_nick = m_nickOriginal + QString::number(m_extension);
    m_extension++;
}
