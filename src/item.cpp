#include <reference.hpp>
#include <item.hpp>
#include <resolver.hpp>
#include <utils.hpp>

#define FORMAT_SEPARATOR ";"
#define FORMAT_NEW_LINE "\r\n"

using Kyrys::Item;

using Kyrys::Resolver;
using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Utils::Random;

Item::Item(const QJsonObject &json)
        :
        m_name(),
        m_nick(),
        m_nickOriginal(),
        m_passwordHash() {
    m_methodType = MethodType::UNKNOWN;
    m_extension = 0;
    m_ID = 0;
    parse(json);
}

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

int Item::isValid() const {
    if (m_methodType == MethodType::INVALID_CMND)
        return Status::INVALID_CMND;
    if (m_methodType == MethodType::UNKNOWN)
        return Status::UNKNOWN_METHOD;

    if (m_methodType == MethodType::REGISTER) {
        if (m_name.isEmpty() || m_nick.isEmpty() || m_passwordHash.isEmpty()) {
            return Status::INVALID_CMND;
        }
    }

    if (m_methodType == MethodType::LOGIN) {
        if (m_nick.isEmpty() || m_passwordHash.isEmpty()) {
            return Status::INVALID_CMND;
        }
    }

    return Status::SUCCESS;
}

//Getters
const MethodType &Kyrys::Item::getMethodType() const { return m_methodType; }

const QString &Kyrys::Item::getName() const { return m_name; }

const QString &Kyrys::Item::getNick() const { return m_nick; }

int Item::getID() const { return m_ID; }

void Item::increaseNick() {
    m_nick = m_nickOriginal + QString::number(m_extension);
    m_extension++;
}

void Item::setID(int ID) {
    m_ID = ID;
}

Item::Item()
        :
        m_name(),
        m_nick(),
        m_nickOriginal(),
        m_passwordHash() {
    m_methodType = MethodType::UNKNOWN;
    m_extension = 0;
    m_ID = 0;
}

const QString &Item::getPasswordHash() const {
    return m_passwordHash;
}

void Item::parse(const QJsonObject &json) {
    if (json["method"].toString().isEmpty()) {
        m_methodType = MethodType::INVALID_CMND;
    } else if (json["method"].toString() == "register") {
        QJsonObject args = json["args"].toObject();

        if (args.empty()) {
            m_methodType = MethodType::INVALID_CMND;
        } else {
            m_methodType = MethodType::REGISTER;
        }

        m_name = args["name"].toString();
        m_nick = args["nick"].toString();
        m_passwordHash = args["password"].toString();

        m_nickOriginal = m_nick;

    } else if (json["method"].toString() == "login") {
        QJsonObject args = json["args"].toObject();
        if (args.empty()) {
            m_methodType = MethodType::INVALID_CMND;
        } else {
            m_methodType = MethodType::LOGIN;
        }
        m_nick = args["nick"].toString();
        m_passwordHash = args["password"].toString();
    } else {
        m_methodType = MethodType::UNKNOWN;
    }
}


