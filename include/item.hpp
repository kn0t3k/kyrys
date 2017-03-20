#pragma once

#include <reference.hpp>
#include <utils.hpp>

namespace Kyrys {
class Item {
	typedef Kyrys::Enums::Item::MethodType MethodType;
	typedef Kyrys::Enums::Resolver::Mode Mode;
	typedef Kyrys::Utils::Random Random;

public:
	/*
	*	
	*/
	Item(const QJsonObject &json);
	std::string Serialize() const ;
	int IsValid() const ;	

	const MethodType &Method() const { return mMethodType;}
	const QString &Name() const { return mName;}
	const QString &Nick() const { return mNick;}

private:
	MethodType mMethodType;
	QString mName;
	QString mNick;

	Random rd;
};
}
