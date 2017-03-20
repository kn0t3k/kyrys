#include <reference.hpp>
#include <resolver.hpp>

#include <fstream>
#include <ctime>

using Kyrys::Resolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;

int Resolver::Execute(const Item &item) {
	if (item.Method() == MethodType::REGISTER) {
		return (this->Register(item));
	} else if (item.Method() == MethodType::UNKNOWN) {
		return Status::UNKNOWN_METHOD;
	} else {
		return Status::INVALID_CMND;
	}
}

int Resolver::Parse(const QString &data, Mode m) {
	if (m == Mode::USE_JSON) {
		// input data is JSON string
		// parse this string
		QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());

		if (d.isNull())	// fail - invalid JSON
			return Status::INVALID_JSON;

		QJsonObject object = d.object();

		Item item(object);

		return this->Execute(item);
	} else {
		qDebug() << "Invalid Resolver mode";
		return Status::FAILED;
	}
}

int Resolver::Register(const Item &item) {
	int s = item.IsValid();
	if (s != Status::SUCCESS) {
		return s;
	}

	std::fstream file;
	std::string fileLoc(TEMP_DIR);
	fileLoc += "/db.txt";
	file.open(fileLoc, std::ios::app);

	if (file.is_open()) {
		file << item.Serialize();
		file.close();
		return Status::SUCCESS;
	}
	return Status::FAILED;
}
