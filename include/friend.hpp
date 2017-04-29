#pragma once
#include <reference.hpp>


namespace Kyrys {
	/**
	 * @brief	class Friend represents one user in user's friend list. This class obtain informations about friend as ID, IP adress, history of communication
	 *
	 */
	class Friend {

	private:
		unsigned int 	m_ID;			//ID of friend - primary key in database of all user's of Kyrys project
		std::string 	m_nickname;		//Nickname of friend
		std::string		m_history_path;	//Holds path to file with encrypted history of communication with friend

	public:


	};
}