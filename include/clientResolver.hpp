#pragma once
#include <reference.hpp>
#include <item.hpp>

#define DEBUGGING_CLIENT_RESOLVER
#ifdef DEBUGGING_CLIENT_RESOLVER

namespace Kyrys {
	class ClientResolver {
		typedef Kyrys::Enums::Item::MethodType MethodType;
		typedef Kyrys::Enums::Resolver::Mode Mode;
		typedef Kyrys::Item Item;

	private:
		QString m_path;

	private:
		/**
		 * @brief
		 * @param item
		 * @return
		 */
		int execute(const Item &item);

		/**
		 * @brief		Registers new user into servers database
		 * @param item
		 * @return
		 */
		int registerItem(const Item &item);

	public:
		/**
		 * @brief      Constructor creates a resolver object.
		 *
		 * @param[in]  path  The path to the directory which should contain database file.
		 */
		ClientResolver(const QString &path);

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
#endif
