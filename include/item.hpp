#pragma once

#include <reference.hpp>

namespace Kyrys {
    class Item {

        typedef Kyrys::Enums::Item::MethodType MethodType;
        typedef Kyrys::Enums::JsonMessage::MessageType MessageType;
		typedef Kyrys::Enums::Chat::Accessibility 		Accessibility;
		typedef Kyrys::Enums::Chat::DataEncryption 		Encryption;

    private:
        MethodType m_methodType;
        MessageType m_messageType;    //Type of message e.g. RegisterRequest, Response, ...
        QString m_nick;                //modified nickname
        QString m_nickOriginal;        //origin nickname
        QString m_passwordHash;        //hash of user's password
        QString m_forwardTo;        //to whom the message shall be forwarded
        QString m_args;            //the args of the message, only the receiving user will be able to de-crypt this
        int m_extension;            //???
        int m_ID;                    //ID of user assigned by server during registration process
        bool m_nick_modified;        //flag if nickname was modified by
        bool m_success;                //flag if process was succesfully finished on server's side

		//WARNING new atributes added: check their meaning in chat.hpp JSON messages EXAMPLES
		unsigned int m_fromID;
		unsigned int m_toID;
		unsigned int m_toNick;
		Accessibility m_Accessibility;	//Holds flag if user is ready for chat means online or is chatting or is offline/"dont want to be disturbed"
		Encryption	  m_Encryption;		//Holds flag if chat will be encrypted against the server or not.


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

        void parseChatRequest(const QJsonObject &json); //todo

        void parseChatResponse(const QJsonObject &json); //todo

        void parseChatData(const QJsonObject &json); //todo

    public:
        //Constructors
        /**
         * Default constructor, its purpose is mainly to reset item values.
         */
        Item();

        //Getters
        const MethodType &getMethodType() const;

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

        //Validating JSON messages
        int isValidRegisterRequest() const;

        int isValidRegisterResponse() const;

        int isValidLoginRequest() const;

        int isValidLoginResponse() const;

        int isValidForward() const;

        int isValidChatSourceDest() const; //Controls if chat message has FROM and TO params

        int isValidChatRequest() const; //todo

        int isValidChatResponse() const; //todo

        int isValidChatData() const; //todo


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

        /**
         * @brief		Resets Item to default state by rewritting all member atributes to default values
         * 				all atributes of type:
         * 				bool 			-> set to false
         * 				integral types  -> set to zero
         * 				string types    -> set to empty string "" with zero length
         * 				1B char types   -> ASCII value is set to zero which represents first char of ASCII table
         */
        void clear();
    };
}
