#include <reference.hpp>
#include <clientResolver.hpp>

using Kyrys::ClientResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;


//Constructors
ClientResolver::ClientResolver(): m_path(""), m_item() {}

//Getters
const Item &ClientResolver::getItem() const { return m_item; }

//Setters
//none


//Other methods
int ClientResolver::execute() { //I will probably delete this method
	if(DEBUG)std::cout << "\nClientResolver::execute called" << std::endl;
	switch (m_item.getMethodType()){
		case (MethodType::INVALID_CMND) : return Status::INVALID_CMND;
		case (MethodType::UNKNOWN) 		: return Status::UNKNOWN_METHOD;
		case (MethodType::REGISTER)		: return Status::SUCCESS;
		case (MethodType::LOGIN)		: return Status::SUCCESS;
		default							: return Status::FAILED;
	}
}


int ClientResolver::parse(const QString &data, Mode m) {
if(DEBUG)std::cout << "\nClientResolver::parse called" << std::endl;
	if (m == Mode::USE_JSON) {

		QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());	// input data is JSON message in QString
		if(DEBUG)std::cout << "\nClient::Resolver - JSON is empty : " << data.isEmpty() << std::endl;
		if(DEBUG)std::cout << "\nClient::Resolver - JSON is empty : " << jsonDoc.isEmpty() << std::endl;
		if (jsonDoc.isNull())
			return Status::INVALID_JSON;								// fail - invalid JSON


		// Parsing object
		m_item.parse(jsonDoc.object());

		//Validation of parsed JSON message
		int validValue = m_item.isValid();
		if(validValue != Status::SUCCESS)
			return validValue;

		return this->execute();
	} else
		return Status::FAILED;
}

