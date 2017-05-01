#pragma once
#include <reference.hpp>
#include <friend.hpp>
#include <QtNetwork/QSslSocket>


namespace Kyrys {
	/**
	 * @brief	class Chat holds informations and functionality about chat between user and his/her friends
	 * @warning	Don't switch receiver and sender. User which initiate chat will be always sender and called
	 * 			user will be always receiver, even in receiver sending answer he will be still chatReceiver.
	 * @warning Each JSON message send by Chat must have fromID and toID parameters! Reason is easer forwarding
	 * 			of messages by server
	 */
	class Chat {

	typedef Kyrys::Enums::JsonMessage::MessageType 	MessageType;
	typedef Kyrys::Enums::Chat::Accessibility 		Accessibility;
	typedef Kyrys::Enums::Chat::DataEncryption 		Encryption;
	typedef Kyrys::Friend Friend;

	private:
		Friend m_ChatSender;   			//Is ALWAYS user which is INITIATING chat !!!
		Friend m_ChatReceiver; 			//Is user which received chat request
		//todo: create sharedkey for encryption by symetric cipher
		Accessibility m_Accessibility;	//Holds flag if user is ready for chat means online or is chatting or is offline/"dont want to be disturbed"
		Encryption	  m_Encryption;		//Holds flag if chat will be encrypted against the server or not.
		//reference vector callHistory	//SK: List uzivatelov s ktorymi som si chatoval aj takych, ktory niesu priatelia
		//reference vector friendList   //List of my friends
		QSslSocket *m_socket = nullptr;




	public:

		Chat();

		Chat(QSslSocket* socket, const Friend &ChatSender, const Friend &ChatReceiver);


		/**
		 * @brief	Sets all atributes of class to default values;
		 */
		void clear();

		int createNewChat(); //todo: this method will be probably deleted, I am not sure yet


		/**
		 * @brief					This message initiate Chat
		 * @param[in] 	recipient	is user which will obtain this message
		 * @return					Returns created message
		 * @note	EXAMPLE of CHAT_REQUEST message
		 * 		   {
         * 		   		"messageType": "CHAT_REQUEST",									//String
                        "method": "chat",												//string
                        "args" : {
                        	"fromID": "ID of user which is sending this message"		//unsigned int
                            "toID": "ID of user to whom is addressed this message", 	//Unsigned int
                            "toNick": "Nick of user to whom is adressed this message", 	//string
                        	"dataEncryption": PLAIN_TEXT / SHARED_KEY,					//Enum integer - this parameter saying if chat will have data encrypted by shared-key or not
                        }
                   }
         * @warning  Keep strictly format of message in validation and parsing methods, especcialy format of keys and type of values!
         * @note	 Valid message need just one of parameters toID or toNick
		 */
		QJsonDocument jsonCreateChatRequest(const Friend& recipient) const; //added


		/**
		 * @brief				This message is answer to CHAT_REQUEST message
		 * @param[in] recipient	is user which sended CHAT_REQUEST parsed from this message
		 * @param[in] answer	true if receiver accepted CHAT_REQUEST, false otherwise
		 * @return				Returns created message
		 * @note	EXAMPLE of CHAT_RESPONSE message:
		 * 		   {
         * 		   		"messageType": "CHAT_RESPONSE",								//String
                        "method": "chat",											//string
                        "args" : {
                            "fromID": "ID of user which is sending this message"	//unsigned int
                            "toID": "ID of user to whom is addressed this message", //Unsigned int
							"accessibility": "ONLINE / OFFLINE / CHATING",			//Enum integer
                            "answer": true/false									//bool
                        }
                   }
         * @warning  Keep strictly format of message in validation and parsing methods, especcialy format of keys and type of values!
		 */
		QJsonDocument jsonCreateChatResponse(const Friend& recipient, bool answer = true) const; //todo


		//todo: rewrite not so clear brief of this comment
		/**
		 * @brief			This message is transporting user's chat in data part of message
		 * @param recipient
		 * @return
		 * @note	EXAMPLE of CHAT_DATA message:
		 * 		   {
         * 		   		"messageType": "CHAT_DATA",									//String
                        "method": "chat",											//string
                        "args" : {
                            "fromID": "ID of user which is sending this message"	//unsigned int
                            "toID": "ID of user to whom is addressed this message", //Unsigned int
							"data": "text of chat"									//string
                        }
                   }
		 */
		QJsonDocument jsonCreateChatData(const Friend &from, const Friend &recipient, const QString &data) const;


		int callID(unsigned int fromID, unsigned int toID);


		//will make hand
		int handshake();
	};
}
