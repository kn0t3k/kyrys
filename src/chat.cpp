#include <reference.hpp>
#include <chat.hpp>

using Kyrys::Chat;


//Constructors

Chat::Chat() {
	clear();
}


/*Chat::Chat(const Kyrys::Friend &m_ChatSender,
		   const Kyrys::Friend &m_ChatReceiver,
		   Kyrys::Chat::Accessibility m_Accessibility,
		   Kyrys::Chat::Encryption m_Encryption) : m_ChatSender(m_ChatSender),
												   m_ChatReceiver(m_ChatReceiver),
												   m_Accessibility(m_Accessibility),
												   m_Encryption(m_Encryption) {}
*/
//Setters
void Chat::clear(){
	m_ChatSender = Friend();
	m_ChatReceiver = Friend();
	m_Accessibility = Accessibility::ONLINE;
	m_Encryption = Encryption::PLAIN_TEXT;
}

int Chat::createNewChat(){
	//todo
	return 0;
}

//Methods creating JSON messages for chat
QJsonDocument Chat::jsonCreateChatRequest(const Friend& recipient) const{
	//todo
	return QJsonDocument();
}

QJsonDocument Chat::jsonCreateChatResponse(const Friend& recipient, bool answer) const{
	//todo
	return QJsonDocument();
}

QJsonDocument Chat::jsonCreateChatData(const Friend& recipient) const{
	//todo
	return QJsonDocument();
}