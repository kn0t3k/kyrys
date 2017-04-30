#include <reference.hpp>
#include <friend.hpp>


using Kyrys::Friend;


//Constructors
Friend::Friend(){ clear(); }

//Friend::Friend(unsigned int ID, const string &nickname, const string &history_path) : m_ID(ID), m_nickname(nickname), m_history_path(history_path) {}


//Setters
void Friend::clear(){
	m_ID = 0;
	m_nickname = "";
}

unsigned int Friend::getID() const {
	return m_ID;
}

const string &Friend::getNickname() const {
	return m_nickname;
}


//Getters
//none yet