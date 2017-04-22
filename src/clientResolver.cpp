#include <reference.hpp>
#include <clientResolver.hpp>


#ifdef test

using Kyrys::ClientResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;


//Constructors
ClientResolver::ClientResolver(const QString &path) : m_path(path) {}

//Getters
//none

//Setters
//none

int ClientResolver::execute(const Item &item) {
	if (item.method() == MethodType::REGISTER)
		return (registerItem(item));
	if (item.method() == MethodType::UNKNOWN)
		return Status::UNKNOWN_METHOD;
	else
		return Status::INVALID_CMND;
}

int ClientResolver::parse(const QString &data, Mode m) {
	if (m == Mode::USE_JSON) {
		// input data is JSON string
		// parse this string
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

		if (jsonDoc.isNull())	// fail - invalid JSON
			return Status::INVALID_JSON;

		QJsonObject object = jsonDoc.object();

		Item item(object); //WTF?? So who is parser? It is ClientResolver::parse or Item constructor?

		return this->execute(item);
	} else
		return Status::FAILED;
}

int ClientResolver::registerItem(const Item &item) {

	//First we control if "item" is on list of implemented methods
	int itemStatus = item.isValid();
	if (itemStatus != Status::SUCCESS) {
		return itemStatus;
	}

	QFile databaseFile(m_path + "/db.txt");			//filePath is path to database, BUT DATABASE OF WHAT??? WTF this is not a filepath but file !!!!!!!!

	if (databaseFile.open(QIODevice::ReadOnly)) { 	//opens database for reading
		QTextStream fileStream(&databaseFile);	  	//send content of database into some sort of QT stringstream
		int ID = 0;
		while ( !fileStream.atEnd()) {				//iterating over lines of file
			fileStream.readLine();					//reads a line from file
			ID++;									//counting ID of what? Last entry?
		}

		databaseFile.close();						//close the file

		if (databaseFile.open(QIODevice::WriteOnly | QIODevice::Append)) {	//Ok now we appending something at the end of database file
			databaseFile.write(item.serialize(ID).c_str());					//serialize creates one new line with one new user's nick password and writes it into database
			databaseFile.close();											//closes the database
			return Status::SUCCESS;
		} else {
			return Status::FAILED;
		}
	}
	return Status::FAILED;
}

#endif
