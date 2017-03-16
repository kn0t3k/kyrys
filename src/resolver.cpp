#include <resolver.hpp>

#include <reference.hpp>

using Kyrys::Resolver;
using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Resolver::MethodType;
using Kyrys::Enums::Resolver::Mode;


int Resolver::Execute(const Item &item) {
	if (item.Method() == MethodType::REGISTER) {
		this->Register(item);
	} else {
		return Status::UNKNOWN_METHOD;
	}
	return Status::SUCCESS;
}

int Resolver::Parse(const QString &data, Mode m) {
	if (m == Mode::USE_JSON) {
		// input data is JSON string
		// parse this string
		QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());

		if (d.isNull())	// fail - invalid JSON
			return Status::INVALID_JSON;

		QJsonObject object = d.object();

		Resolver::Item item(object);

		return this->Execute(item);
	} else {
		qDebug() << "Invalid Resolver mode";
		return Status::FAILED;
	}
}

int Resolver::Register(const Item &item) {
	return Status::SUCCESS;
}

Resolver::Item::Item(const QJsonObject &json) {
	if (json["method"].toString() == "register") {
		this->mMethodType = MethodType::REGISTER;
	} else if (json["method"].toString() == "call") {
		this->mMethodType = MethodType::CALL;
	} else {
		this->mMethodType = MethodType::UNKNOWN;
	}

	QJsonObject obj = json["args"].toObject();
	this->mName = obj["name"].toString();
	this->mNick = obj["nick"].toString();
	this->ID = -1;
}