#pragma once
#include <reference.hpp>

namespace Kyrys {
class Resolver {
	typedef Kyrys::Enums::Resolver::MethodType MethodType;
	typedef Kyrys::Enums::Resolver::Mode Mode;

	struct Item;

public:	
	int Parse(const QString &data, Mode m);

private:
	int Execute(const Item& item);
	int Register(const Item &item);

	struct Item {
	public:
		Item(const QJsonObject &json);
		MethodType Method() const { return mMethodType;}
	private:
		MethodType mMethodType;
		QString mName;
		QString mNick;
		int ID;
	};

};
}