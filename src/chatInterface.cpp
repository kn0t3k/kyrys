
#include <iostream>

#include <chatInterface.hpp>
#include <clientResolver.hpp>
#include <friend.hpp>
#include <chat.hpp>

#define CLIENT_PROMPT ">"
#define CHAT_PROMPT ">>"

using Kyrys::ChatInterface;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Client::Registration::PasswordSecQuality;
using Kyrys::ClientResolver;
using Kyrys::Friend;
using Kyrys::Chat;

void ChatInterface::commandLoop() {
    std::string command;
    do {
        std::cout << "\n> " << std::flush;
        std::getline(std::cin, command);
        if (command == "help") {
            //todo: writes some man page on stdout, use welcomepages.hpp
            continue;
        }
        if (command == "register") {
            registration();
            continue;
        }
        if (command == "login") {
            login();
            continue;
        }
        if (command == "chat") {
            chatLoop();
            continue;
        }

        std::cout << "\nUNKNOWN COMMAND\n" << std::endl;
    } while (command != "quit");
    emit clientChatInterfaceQuit();
}

ChatInterface::ChatInterface(Client *parent)
        :
        m_clientParent(parent) {

}

void ChatInterface::login() {
    std::string nickname;
    std::string password;

    for (int i = 0; i < 5; ++i) {
        //Loading login credentials
        loadLoginCredentials(nickname, password, std::cin);

        //Hashing password
        QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), HASH);
        m_user = User(nickname, hashed_password);
        hashed_password.clear();

        //Sending LOGIN_REQUEST
        QByteArray loginRequest = jsonMessageUserAuthentication(
                MessageType::LOGIN_REQUEST).toJson(); //this is message for server
        if (!m_clientParent->send(loginRequest)) {
            if (DEBUG)std::cout << "\nClient::registration - LOGIN_REQUEST message - FAIL" << std::endl;
        }

        //Receiving REGISTER_RESPONSE
        QByteArray loginResponse = m_clientParent->receive();
        qDebug() << "login response: " << loginResponse;
        if (loginResponse.isEmpty()) {
            if (DEBUG)std::cout << "\nClient::registration - LOGIN_RESPONSE message - FAIL" << std::endl;
        }

        //Parsing LOGIN_RESPONSE message
        ClientResolver clientResolver;
        int returnState = clientResolver.parse(loginResponse);

        if (returnState != Status::FAILED) {
            if (DEBUG)std::cout << "\ngetSuccess = " << clientResolver.getItem().getSuccess() << std::endl;
            if (clientResolver.getItem().getSuccess()) {
                std::cout << "\nUser: " << m_user.getNickname() << "was succesfully logged in" << std::endl;
                break;
            } else {
                std::cout << "\nLogin failed - try it again" << std::endl;
                continue;
            }
        }
    }
}

void ChatInterface::registration(std::istream &in) {
    std::cout << "\nNow follows registration procedure" << std::endl;

    std::string nickname;
    std::string password;

    //Loading nickname and password
    if (loadRegistrationCredentials(nickname, password, in) == rStatus::BAD_PASSWORD) {
        std::cout << "\nRegistration failed. Check help page and try it again" << std::endl;
        return;
    }

    //Hashing password
    const QByteArray hashed_password = QCryptographicHash::hash(QByteArray::fromStdString(password), HASH);
    m_user = User(nickname, hashed_password);

    //Sending REGISTER_REQUEST
    QByteArray registerRequest = jsonMessageUserAuthentication(MessageType::REGISTER_REQUEST).toJson();
    if (!m_clientParent->send(registerRequest)) {
        if (DEBUG) std::cout << "\nClient::registration - REGISTER_REQUEST message - FAIL" << std::endl;
        return;
    }

    //Receiving REGISTER_RESPONSE
    QByteArray registerResponse = m_clientParent->receive();
    if (registerResponse.isEmpty()) {
        if (DEBUG) std::cout << "\nClient::registration - REGISTER_RESPONSE message - FAIL" << std::endl;
        return;
    }

    //Parsing REGISTER_RESPONSE message
    ClientResolver clientResolver;
    int returnState = clientResolver.parse(registerResponse);

    if (returnState != Status::FAILED) {
        if (DEBUG) std::cout << "\ngetSuccess = " << clientResolver.getItem().getSuccess() << std::endl;
        if (clientResolver.getItem().getSuccess()) {
            copyRegistrationItem(clientResolver.getItem());
            std::cout << "\nNew user was succesfully registered\n" << std::endl;
            m_user.printUser();
        } else {
            std::cout << "\nRegistration failed" << std::endl;
            return;
        }
    }
}

int ChatInterface::controlPasswordSecQuality(const std::string &password) const {
    if (password.length() >= 8)
        return PasswordSecQuality::GOOD;
    else
        return PasswordSecQuality::TOO_SHORT;
}

void ChatInterface::copyRegistrationItem(const Item &item) {
    m_user.setID(item.getID());
    m_user.setNickname(item.getNick().toStdString());
}

QJsonDocument ChatInterface::jsonMessageUserAuthentication(MessageType messageType) {
    QJsonObject args_obj;
    QJsonObject root_obj;

    if (messageType == MessageType::REGISTER_REQUEST) {
        root_obj["messageType"] = "REGISTER_REQUEST";
        root_obj["method"] = "register";
    } else {
        root_obj["messageType"] = "LOGIN_REQUEST";
        root_obj["method"] = "login";
    }

    args_obj["nickname"] = QString::fromStdString(m_user.getNickname());
    args_obj["password"] = m_user.getPasswordHash().toStdString().c_str();
    args_obj["hash algorithm"] = HASH;

    root_obj.insert("args", args_obj);

    // todo add - salt of random values at the end of JSON message
    //		    - ID of message (maybe)

    return QJsonDocument(root_obj);
}

//Other methods
int ChatInterface::loadRegistrationCredentials(std::string &nickname, std::string &password, std::istream &in) {
    std::string nicknameBuffer;
    std::cout << "\nChoose nickname     :";
    std::getline(in, nicknameBuffer);

    std::string passwordBuffer1;
    std::string passwordBuffer2;

    for (int i = 0; i < 5; ++i) { //User has 5 tries to type password correctly twice in a row
        std::cout << "\nChoose password     :";
        std::getline(in, passwordBuffer1);
        if (controlPasswordSecQuality(passwordBuffer1) != PasswordSecQuality::GOOD) {
            std::cout << "\nPassword is too short\nChoose password at least 8 characters long!" << std::endl;
            passwordBuffer1.clear();
            continue;
        } else {
            std::cout << "Repeat the password :";
            std::getline(in, passwordBuffer2);
            if (passwordBuffer1 != passwordBuffer2) {
                std::cout << "\nPasswords are not same!, please try it again" << std::endl;
                passwordBuffer1.clear();
                passwordBuffer2.clear();
                continue;
            } else {
                nickname = nicknameBuffer;
                password = passwordBuffer1;
                passwordBuffer1.clear(); //security precautions
                passwordBuffer2.clear();
                return rStatus::SUCCESS;
            }
        }
    }
    return rStatus::BAD_PASSWORD;
}

int ChatInterface::loadLoginCredentials(std::string &nickname, std::string &password, std::istream &in) {
    std::cout << "Nickname: ";
    std::getline(in, nickname);
    std::cout << "\nPassword: ";
    std::getline(in, password);
    return 0;
}

void ChatInterface::printMessage(const Item &incomingMessage) {
    if (incomingMessage.isValid())
        std::cout << "\n" << CHAT_PROMPT << " [FROM ID " << incomingMessage.getFromID() << "]: "
                  << incomingMessage.getData().toStdString() << std::endl;
}

void ChatInterface::sendTo() {

    //Preparing fromID, toID and inicializing Chat class
    std::cout << "\n" << CHAT_PROMPT << " to ID:" << std::flush;
    unsigned int toID;
    std::string buffer;
    std::getline(std::cin, buffer);
    toID = static_cast<unsigned int>(std::stoul(buffer));
    Friend sender(m_user.getID());
    Friend receiver(toID);
    Chat chat(sender, receiver);

    //Preparing CHAT_DATA message and sending it to server
    std::cout << "\n" << CHAT_PROMPT << " [You]: " << std::flush;
    getline(std::cin, buffer);
    QString chatData = chat.jsonCreateChatData(sender, receiver, QString::fromStdString(buffer)).toJson();
    m_clientParent->send(chatData);
}

void ChatInterface::chatLoop() {
    std::string command;

    emit toggleAsyncSocketRead();

    do {
        std::cout << "\n" << CHAT_PROMPT << " " << std::flush;
        std::getline(std::cin, command);

        if (command == "#callid") {
            //todo
            continue;
        }

        if (command == "#callnick") {
            //todo
            continue;
        }

        if (command == "#history") {
            //todo
            continue;
        }

        if (command == "#sendto") {
            sendTo();
            //todo: ak je druhy klient v chat interface, tak sa mu pernamentne bez nadviazavania spojenia zobrazi sprava
            continue;
        }

        if (command == "#quit") {
            emit toggleAsyncSocketRead();
            break;
        }

        std::cout << "\nUNKNOWN CHAT COMMAND\n" << std::endl;
    } while (command != "#quit");
}
