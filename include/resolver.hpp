#pragma once
#include <reference.hpp>
#include <item.hpp>

namespace Kyrys {
class Resolver {
	typedef Kyrys::Enums::Item::MethodType MethodType;
	typedef Kyrys::Enums::Resolver::Mode Mode;
	typedef Kyrys::Item Item;

private:
	QString m_path;

	int execute(const Item &item);
	int registerItem(const Item &item);

public:
	/**
	 * @brief      Construct a resolver object.
	 *
	 * @param[in]  path  The path to the directory which should contain database file.
	 */
	Resolver(const QString &path);

	/**
	 * @brief      Parse input string using specified mode.
	 *
	 * @param[in]  data  The data to parse.
	 * @param[in]  m     The mode in which the resolver operates.
	 *
	 * @return     Return success if succeeded, fail otherwise.
	 */
	int parse(const QString &data, Mode m);
};
}