#include <reference.hpp>
#include <resolver.hpp>

using Kyrys::Resolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;

Resolver::Resolver(const QString &path) : m_path(path) {}

int Resolver::execute(const Item &item) {
	if (item.method() == MethodType::REGISTER) {
		return (this->registerItem(item));
	} else if (item.method() == MethodType::UNKNOWN) {
		return Status::UNKNOWN_METHOD;
	} else {
		return Status::INVALID_CMND;
	}
}

int Resolver::parse(const QString &data, Mode m) {
	if (m == Mode::USE_JSON) {
		// input data is JSON string
		// parse this string
		QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());

		if (d.isNull())	// fail - invalid JSON
			return Status::INVALID_JSON;

		QJsonObject object = d.object();

		Item item(object);

		return this->execute(item);
	} else {
		return Status::FAILED;
	}
}

int Resolver::registerItem(const Item &item) {
	int s = item.isValid();
	if (s != Status::SUCCESS) {
		return s;
	}

	QFile filePath(m_path + "/db.txt");
	if (filePath.open(QIODevice::ReadOnly)) {
		QTextStream fileStream(&filePath);
		int ID = 0;
		while ( !fileStream.atEnd()) {
			fileStream.readLine();
			ID++;
		}

		filePath.close();

		if (filePath.open(QIODevice::WriteOnly | QIODevice::Append)) {
			filePath.write(item.serialize(ID).c_str());
			filePath.close();
			return Status::SUCCESS;
		} else {
			return Status::FAILED;
		}
	}
	return Status::FAILED;
}
