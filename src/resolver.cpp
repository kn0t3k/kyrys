#include <reference.hpp>
#include <resolver.hpp>

using Kyrys::Resolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;

Resolver::Resolver(const QString &path) : mPath(path) {

}

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
		return Status::FAILED;
	}
}

int Resolver::Register(const Item &item) {
	int s = item.IsValid();
	if (s != Status::SUCCESS) {
		return s;
	}

	QFile filePath(mPath + "/db.txt");
	if (filePath.open(QIODevice::ReadWrite )) {
		int ID = 0;

		QTextStream fileStream(&filePath);
		QVector<QString > lines;
		while ( !fileStream.atEnd()) {
			lines.append(fileStream.readLine());
		}

		if (lines.length() == 0) {
			lines.append(QString::number(ID));
		}

		ID = lines[0].toInt();
		ID++;

		lines.append(QString::fromStdString(item.Serialize(ID)));
		lines[0] = QString::number(ID);

		filePath.close();

		if (filePath.open(QIODevice::WriteOnly)) {
			QTextStream out(&filePath);
			for (QVector<QString>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
				out << *iter << "\r\n";
			}
			return Status::SUCCESS;
		} else {
			return Status::FAILED;
		}
	}
	return Status::FAILED;
}
