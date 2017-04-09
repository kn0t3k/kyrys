#pragma once

#include <reference.hpp>

namespace Kyrys {
class Item {
	typedef Kyrys::Enums::Item::MethodType MethodType;
	typedef Kyrys::Enums::Resolver::Mode Mode;

public:
	/**
	 * @brief      Constructor of item, construct abstract item which holds info
	 * about what should execute etc.
	 *
	 * @param[in]  json  The json is input json object containing data to populate item with.
	 */
	Item(const QJsonObject &json);

	/**
	 * @brief      Serializes item content to string. Good for saving to file.
	 *
	 * @param[in]  ID    ID of new entry in DB.
	 *
	 * @return     Returns serialized string.
	 */
	std::string serialize(int ID) const ;

	/**
	 * @brief      Checks whether an item is valid, ie. contains invalid method name, empty nick or name 
	 * or any other invalid data. Can vary depending on which method is to be used.
	 *
	 * @return     True if valid, False otherwise.
	 */
	int isValid() const ;

	const MethodType &method() const { return m_methodType;}
	const QString &name() const { return m_name;}
	const QString &nick() const { return m_nick;}

private:
	MethodType m_methodType;
	QString m_name;
	QString m_nick;
};
}
