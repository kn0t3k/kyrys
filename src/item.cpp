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

Item::Item(const QJsonObject &json) {
	if (json["method"].toString().isEmpty()) {
		this->m_methodType = MethodType::INVALID_CMND;
	} else if (json["method"].toString() == "register") {
		this->m_methodType = MethodType::REGISTER;
	} else if (json["method"].toString() == "call") {
		this->m_methodType = MethodType::CALL;
	} else {
		this->m_methodType = MethodType::UNKNOWN;
	}

	QJsonObject obj = json["args"].toObject();

	if (obj.empty()) {
		this->m_methodType = MethodType::INVALID_CMND;
	}

	this->m_name = obj["name"].toString();
	this->m_nick = obj["nick"].toString();
}

std::string Item::serialize(int ID) const {
	std::string output = std::to_string(ID);

	output += FORMAT_SEPARATOR;
	output += m_name.toStdString();
	output += FORMAT_SEPARATOR;
	output += m_nick.toStdString();
	output += FORMAT_NEW_LINE;
	return output;
}

int Item::isValid() const {
	if (m_methodType == MethodType::INVALID_CMND)
		return Status::INVALID_CMND;
	if (m_methodType == MethodType::UNKNOWN)
		return Status::UNKNOWN_METHOD;

	if (m_methodType == MethodType::REGISTER) {
		if (m_name.isEmpty() || m_nick.isEmpty()) {
			return Status::INVALID_CMND;
		}
	}

	return Status::SUCCESS;
}
