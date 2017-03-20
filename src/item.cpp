#include <reference.hpp>
#include <item.hpp>
#include <resolver.hpp>
#include <utils.hpp>

#include <ctime>

#define FORMAT_SEPARATOR ";"
#define FORMAT_NEW_LINE '\n'

using Kyrys::Item;

using Kyrys::Resolver;
using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Utils::Random;


Item::Item(const QJsonObject &json) {
	if (json["method"].toString().isEmpty()) {
		this->mMethodType = MethodType::INVALID_CMND;
	} else if (json["method"].toString() == "register") {
		this->mMethodType = MethodType::REGISTER;
	} else if (json["method"].toString() == "call") {
		this->mMethodType = MethodType::CALL;
	} else {
		this->mMethodType = MethodType::UNKNOWN;
	}

	QJsonObject obj = json["args"].toObject();

	this->mName = obj["name"].toString();
	this->mNick = obj["nick"].toString();
}

std::string Item::Serialize() const {
	QString name = mName;
	name += rd.GetRandomString();
	unsigned long ID = Random::Hash(name.toLatin1().data());

	std::string output = std::to_string(ID);

	output += FORMAT_SEPARATOR;
	output += mName.toStdString();
	output += FORMAT_SEPARATOR;
	output += mNick.toStdString();
	output += FORMAT_NEW_LINE;
	return output;
}

int Item::IsValid() const {
	if (mMethodType == MethodType::INVALID_CMND)
		return Status::INVALID_CMND;
	if (mMethodType == MethodType::UNKNOWN)
		return Status::UNKNOWN_METHOD;

	if (mMethodType == MethodType::REGISTER) {
		if (mName.isEmpty() || mNick.isEmpty()) {
			return Status::INVALID_CMND;
		}
	}


	return Status::SUCCESS;
}


