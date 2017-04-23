#include <reference.hpp>
#include <clientResolver.hpp>


#ifdef DEBUGGING_CLIENT_RESOLVER

using Kyrys::ClientResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;


//Constructors
ClientResolver::ClientResolver(const QString &path) : m_path(path) {
	m_item = Item(); //test this
}

//Getters
const Item &ClientResolver::getItem() const { return m_item; }

//Setters
//none


//Other methods
int ClientResolver::execute() { //I will probably delete this method
	switch (m_item.getMethodType()){
		case (MethodType::INVALID_CMND) : return Status::INVALID_CMND;
		case (MethodType::UNKNOWN) 		: return Status::UNKNOWN_METHOD;
		case (MethodType::REGISTER)		: return Status::SUCCESS;
		case (MethodType::LOGIN)		: return Status::SUCCESS;
		default							: return Status::FAILED;
	}
}


int ClientResolver::parse(const QString &data, Mode m) {
	if (m == Mode::USE_JSON) {
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());	// input data is JSON message in QString
		if (jsonDoc.isNull())
			return Status::INVALID_JSON;								// fail - invalid JSON
		QJsonObject object = jsonDoc.object();
		m_item = Item(object);	// Parsing object

		//Validation of parsed JSON message
		int validValue = m_item.isValid();
		if(validValue != Status::SUCCESS)
			return validValue;


		return this->execute();
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
