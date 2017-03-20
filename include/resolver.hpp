#pragma once
#include <reference.hpp>
#include <item.hpp>

namespace Kyrys {
class Resolver {
	typedef Kyrys::Enums::Item::MethodType MethodType;
	typedef Kyrys::Enums::Resolver::Mode Mode;
	typedef Kyrys::Item Item;

public:
	int Parse(const QString &data, Mode m);

private:
	int Execute(const Item &item);
	int Register(const Item &item);
};
}