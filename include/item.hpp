#pragma once

#include <reference.hpp>

namespace Kyrys {
    class Item {

        typedef Kyrys::Enums::Item::MethodType MethodType;
        typedef Kyrys::Enums::Resolver::Mode Mode;
     private:
        MethodType m_methodType;
        QString m_name;				//todo: maybe delete this
        QString m_nick;				//modified nickname
        QString m_nickOriginal;		//origin nickname
        QString m_passwordHash;		//hash of user's password
        QString m_forwardTo;    	//to whom the message shall be forwarded
        QString m_args; 			//the args of the message, only the receiving user will be able to de-crypt this
        int m_extension;			//???
        int m_ID;					//ID of user assigned by server during registration process
		bool m_nick_modified;		//flag if nickname was modified by
		int m_success;				//flag if process was succesfully finished on server's side

	public:
		//Parsing system
	    /**
	     * @brief			This is main parsing method covering system of smaller parsing methods with prefix parse.
	     * 					You can see them bellow.
	     *
	     * @param[in]json	json message which will be parsed into class members atributes called same as keynames in message
	     * 					For example key "nick" has member atribute "m_nick"
	     *
	     * @note			If you want to add new parsing method, add it between
	 	 *       			last parsing method FORWARD and statement UNKOWN in this format example
	     *       			It is more clear code than hundredts of else branches with same code UNKNOWN
	     *
	     * "BEGIN OF EXAMPLE"
	 	 * if (json["method"].toString() == "name of method") {
	 	 * 		parseMessageXYZ(json)
	 	 * 		return;  //Don't forget on return!
	 	 * }
	     * "END OF EXAMPLE"
	     *
	 	 */
        void parse(const QJsonObject &json);

	//private: //Because we need to test these methods
		//Parsing JSON messages
		void parseRegisterRequest(const QJsonObject &json);
		void parseLoginRequest(const QJsonObject &json);
		void parseRegisterResponse(const QJsonObject &json);
		void parseLoginResponse(const QJsonObject &json);
		void parseForward(const QJsonObject &json);

	public:
        //Constructors
        /**
         * Default constructor, its purpose is mainly to reset item values.
         */
        Item();

        /**
         * @brief      Parametric constructor of item, construct abstract item which holds info
         *             about what should execute etc.
         *
         * @param[in]  json  The json is input json object containing data to populate item with.
         */
        Item(const QJsonObject &json);


        //Getters
        const MethodType &getMethodType() const;

		const QString &getName() const;

		const QString &getNick() const;

		const QString &getPasswordHash() const;

		const QString &getRecepient() const;

        const QString &getArgs() const;

        int getID() const;

		bool getNick_modified() const;

		bool getSuccess() const;


		//Setters
        void setID(int ID);


		//Validation system
		/**
         * @brief      Checks whether an item is valid, ie. contains invalid method name, empty nick or name
         *             or any other invalid data. Can vary depending on which method is to be used.
         *
         * @return     True if valid, False otherwise.
         */
		int isValid() const; //We will expand and refactor validation system with new methods covering all JSON messages


        //Other methods
        /**
         * @brief      Serializes item content to string. Good for saving to file.
         *
         * @param[in]  ID    ID of new entry in DB.
         *
         * @return     Returns serialized string.
         */
        std::string serialize(int ID) const;

        /**
         * @brief      If nick of the user has already been found in the DB,
         *             add numbers to its end so it it unique.
         */
        void increaseNick();
    };
}
