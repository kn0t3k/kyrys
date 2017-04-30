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
	QJsonObject args_obj;
	QJsonObject root_obj;
	QString key;
	QString value;

	root_obj["messageType"] = "CHAT_REQUEST";
	root_obj["method"] = "chat";

	args_obj["fromID"] = m_ChatSender.getID();
	args_obj["toID"] = recipient.getID();
	args_obj["toNick"] = QString::fromStdString(recipient.getNickname());

	root_obj.insert("args", args_obj);

	return QJsonDocument(root_obj);
}

QJsonDocument Chat::jsonCreateChatResponse(const Friend& recipient, bool answer) const{
	QJsonObject args_obj;
	QJsonObject root_obj;
	QString key;
	QString value;

	root_obj["messageType"] = "CHAT_RESPONSE";
	root_obj["method"] = "chat";

	args_obj["fromID"] = m_ChatReceiver.getID();
	args_obj["toID"] = recipient.getID();
	args_obj["accessibility"] = m_Accessibility;
	args_obj["answer"] = answer;

	root_obj.insert("args", args_obj);

	return QJsonDocument(root_obj);
}

QJsonDocument Chat::jsonCreateChatData(const Friend &from, const Friend &recipient, const QString &data) const{
	QJsonObject args_obj;
	QJsonObject root_obj;

	root_obj["messageType"] = "CHAT_DATA";
	root_obj["method"] = "chat";

	args_obj["fromID"] = from.getID();
	args_obj["toID"] = recipient.getID();
	args_obj["data"] = data;

	root_obj.insert("args", args_obj);

	return QJsonDocument(root_obj);
}