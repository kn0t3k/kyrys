#pragma once
#include <reference.hpp>


namespace Kyrys {
	/**
	 * @brief	class Friend represents one user in user's friend list. This class obtain informations about friend as ID, IP adress(not neccesary), history of communication etc.
	 *
	 */
	class Friend {

	private:
		unsigned int 	m_ID;			//ID of friend - primary key in database of all user's of Kyrys project
		std::string 	m_nickname;		//Nickname of friend
		//std::string		m_history_path;	//Holds path to file with encrypted history of communication with friend
		bool m_chatting;	//This is flag if user Friend created handshake consisting from CHAT_REQUEST and accepting CHAT_RESPONSE

	public:
		//Constructors
		Friend();

		Friend(unsigned int ID);


		//Setters
		void clear();
		void setChatting(bool m_chatting);

		//Getters
		unsigned int getID() const;
		const string &getNickname() const;
		bool isChatting() const;


	};
}