#include <reference.hpp>
#include <friend.hpp>


using Kyrys::Friend;


//Constructors
Friend::Friend(){ clear(); }

Friend::Friend(unsigned int ID) : m_ID(ID) {}

Friend::Friend(unsigned int ID, const std::string& nickname): m_ID(ID), m_nickname(nickname){}

//Setters
void Friend::clear(){
	m_ID = 0;
	m_nickname = "";
	m_chatting = false;
}
void Friend::setChatting(bool m_chatting) { Friend::m_chatting = m_chatting; }


//Getters
unsigned int Friend::getID() const { return m_ID; }

const string &Friend::getNickname() const { return m_nickname; }

bool Friend::isChatting() const { return m_chatting; }

