#include <reference.hpp>
#include <clientResolver.hpp>


#ifdef DEBUGGING_CLIENT_RESOLVER

using Kyrys::ClientResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;


//Constructors
ClientResolver::ClientResolver(const QString &path) : m_path(path) {}

//Getters
const Item &ClientResolver::getItem() const { return m_item; }

//Setters
//none


//Other methods
int ClientResolver::execute(const Item &item) {
	//First we control if "item" is on list of implemented methods
	int itemValidity = item.isValid();
	if (itemValidity != Status::SUCCESS)
		return itemValidity;

	switch (item.getMethodType()){
		case (MethodType::INVALID_CMND) : return Status::INVALID_CMND;
		case (MethodType::UNKNOWN) 		: return Status::UNKNOWN_METHOD;
		case (MethodType::REGISTER)		: return analyzeRegisterResponse(item);
		case (MethodType::LOGIN)		: return analyzeLoginResponse(item);
		default							: return Status::FAILED;
	}
}


int ClientResolver::parse(const QString &data, Mode m) {
	if (m == Mode::USE_JSON) {
		// input data is JSON string
		// parse this string
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

		if (jsonDoc.isNull())	// fail - invalid JSON
			return Status::INVALID_JSON;

		QJsonObject object = jsonDoc.object();

		Item item(object);

		return this->execute(item);
	} else
		return Status::FAILED;
}

int ClientResolver::analyzeRegisterResponse(const Item &item) {
	//todo
	return 0;
}

int ClientResolver::analyzeLoginResponse(const Item &item) {
	//todo
	return 0;
}

#endif
