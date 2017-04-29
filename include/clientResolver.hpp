#pragma once

#include <reference.hpp>
#include <item.hpp>
#include <user.hpp>

namespace Kyrys {
    class ClientResolver {
        typedef Kyrys::Enums::Item::MethodType MethodType;
        typedef Kyrys::Enums::Resolver::Mode Mode;
        typedef Kyrys::Item Item;

    private:
        QString m_path;    // path to database of friends
        Item m_item;    // incoming response from server

    private:
        /**
         * @brief
         * @param item
         * @return
         */
        int execute();

    public:
        //Constructors
        /**
         * @brief      Constructor creates a resolver object.
         *
         * @param[in]  path  The path to the directory which should contain database file.
         */
        ClientResolver();

        //Getters
        const Item &getItem() const;


		//Other methods
        /**
         * @brief      Parse input string using specified mode.
         *
         * @param[in]  data  The data to parse.
         * @param[in]  m     The mode in which the resolver operates.
         *
         * @return     Return success if succeeded, fail otherwise.
         */
        int parse(const QString &data, Mode m = Mode::USE_JSON);

		//Don't delete comment below this yet
        /**
         * @brief		This metod analyze REGISTER_RESPONSE message from server and returns for client result of server's response
         * @param[in] 	item	Item created from RESPONSE_MESSAGE
         * @param[in]	user	Reference to user where method writes data from message
         * @return 		Returns Client::Registration::Status
         * 				SUCCESS = 0		   	- Registration process was finished succesfully on server's side
         *				MODIFIED_NICKNAME   - User wrote nickname which is already registered, so server changed it to similar and unique form
         *				SERVER_ERROR	   		//Uknown error on server's side
         */

    };
}
