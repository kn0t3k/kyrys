#include <reference.hpp>
#include <serverResolver.hpp>

using Kyrys::ServerResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;

//Constructors
ServerResolver::ServerResolver()
        : incomingItem(),
          m_userFromDB("", "") {
    clear();

    QString databasePath = QString(DATABASE_DIRECTORY) + "/" + QString(DATABASE_FILENAME);

    if (!QDir(DATABASE_DIRECTORY).exists()) {
        QDir().mkdir(DATABASE_DIRECTORY);
    }

    m_dbHandle = QSqlDatabase::addDatabase("QSQLITE");
    m_dbHandle.setDatabaseName(databasePath);
    if (!m_dbHandle.open()) {
        qDebug() << "db failed to open at " + databasePath;
        return;
    }

    if (!m_dbHandle.tables().contains(QLatin1String("users"))) {
        QSqlQuery query;
        query.prepare("CREATE TABLE users(ids integer primary key, nickname text)");
        if (!query.exec()) {
            qDebug() << "craete table error:  " << query.lastError();
            return;
        }
    }

    if (!m_dbHandle.tables().contains(QLatin1String("passwords"))) {
        QSqlQuery query;
        query.prepare("CREATE TABLE passwords(ids integer primary key, password text)");
        if (!query.exec()) {
            qDebug() << "craete table error:  " << query.lastError();
            return;
        }
    }
}

//Getters
bool ServerResolver::isForward() { return m_stateIsForward; }

bool ServerResolver::isLogin() { return m_stateIsLogin; }

const Item &ServerResolver::getItem() const { return incomingItem; }

//Other methods
int ServerResolver::execute() {
    m_stateIsForward = false;
    m_stateIsLogin = false;

    int s = incomingItem.isValid();
    if (s != Status::SUCCESS) {
        return s;
    }

    switch (incomingItem.getMethodType()) {
        case MethodType::REGISTER : {
            return registerUser();
        }
        case MethodType::LOGIN : {
            return loginUser();
        }
        case MethodType::UNKNOWN : {
            return Status::UNKNOWN_METHOD;
        }
        case MethodType::CHAT : {
            return forwardChat();
        }
        default: {
            return Status::INVALID_CMND;
        }
    }
}

int ServerResolver::forwardChat() {
    QString toNick = incomingItem.getToNick();
    if (toNick.isEmpty()) {
        m_IDofRecipient = incomingItem.getToID();
    } else {
        m_IDofRecipient = getUserIDFromDB(toNick);
    }
    if (m_IDofRecipient != -1) {
        m_stateIsForward = true;
        return Status::SUCCESS;
    }
    return Status::INVALID_CMND;

}

int ServerResolver::parse(const QString &data, Mode m) {
    clear();
    m_messageDataToForward = data;
    if (m == Mode::USE_JSON) {
        QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());

        if (d.isNull())    // fail - invalid JSON
            return Status::INVALID_JSON;

        QJsonObject object = d.object();

        incomingItem.clear();
        incomingItem.parse(object);

        m_result = execute();

        return m_result;
    } else {
        return Status::FAILED;
    }
}

int ServerResolver::registerUser() {
    QSqlQuery query;

    //check if nick already in database
    //if it does, make it unique
    query.prepare("SELECT ids FROM users WHERE nickname = (:nickname)");
    while (true) {
        query.bindValue(":nickname", incomingItem.getNick());
        if (!query.exec()) {
            qDebug() << "select error:  " << query.lastError();
            return Status::FAILED;
        } else {
            if (query.next()) {
                incomingItem.increaseNick();
            } else {
                break;
            }
        }
    }

    //insert the user into database
    query.prepare("INSERT INTO users (nickname) VALUES (:nickname)");
    query.bindValue(":nickname", incomingItem.getNick());
    if (!query.exec()) {
        qDebug() << "insert error:  " << query.lastError();
        return Status::FAILED;
    }

    query.prepare("SELECT MAX(ids) FROM users");
    if (!query.exec()) {
        qDebug() << "select max ids error:  " << query.lastError();
        return Status::FAILED;
    } else {
        if (query.next()) {
            incomingItem.setID(query.value(0).toInt());
        }
    }

    //insert the user's password into database
    query.prepare("INSERT INTO passwords (password) VALUES (:password)");
    query.bindValue(":password", incomingItem.getPasswordHash());
    if (!query.exec()) {
        qDebug() << "insert error:  " << query.lastError();
        return Status::FAILED;
    }

    return Status::SUCCESS;
}

QByteArray ServerResolver::prepareResponse() {
    QByteArray response;

    switch (incomingItem.getMethodType()) {
        case MethodType::REGISTER : {
            response = prepareRegisterResponse();
            break;
        }
        case MethodType::LOGIN : {
            response = prepareLoginResponse();
            break;
        }
        case MethodType::CHAT : {
            response = m_messageDataToForward.toLocal8Bit();
            break;
        }
        default : {
            break;
        }
    }

    return response;
}

int ServerResolver::getUserIDFromDB(const QString &nickName) {
    int userID = -1;

    QSqlQuery query;
    query.prepare("SELECT ids FROM users WHERE nickname = (:nickname)");
    query.bindValue(":nickname", nickName);
    if (!query.exec()) {
        qDebug() << "select error:  " << query.lastError();
    } else {
        if (query.next()) {
            userID = query.value(0).toInt();
        }
    }

    return userID;
}

void ServerResolver::clear() {
    m_result = Status::FAILED;
    incomingItem = Item();
    m_stateIsForward = false;
    m_stateIsLogin = false;
    m_userFromDB = User("", "");
    m_IDofRecipient = -1;
}

int ServerResolver::loginUser() {
    // get user ID from db, search by nick, set this ID to m_item
    int ID = getUserIDFromDB(incomingItem.getNick());
    if (ID == -1) {
        return Status::INVALID_CRED;
    } else {
        getUserPassword(ID);
        // compare received hash with the one from database
        if (incomingItem.getPasswordHash().toLocal8Bit() == m_userFromDB.getPasswordHash()) {
            incomingItem.setID(ID);
            m_stateIsLogin = true;
        } else {
            return Status::INVALID_CRED;
        }
    }
    return Status::SUCCESS;
}

int ServerResolver::getRecipientID() const {
    return m_IDofRecipient;
}

void ServerResolver::getUserPassword(int userID) {
    QSqlQuery query;
    query.prepare("SELECT password FROM passwords WHERE ids = (:id)");
    query.bindValue(":id", userID);
    if (!query.exec()) {
        qDebug() << "select error:  " << query.lastError();
    } else {
        if (query.next()) {
            QString pass = query.value(0).toString();
            m_userFromDB.setPasswordHash(pass.toLocal8Bit());
        }
    }
}

QByteArray ServerResolver::prepareRegisterResponse() {
    QJsonObject root_obj;
    QJsonObject args_obj;

    root_obj["messageType"] = "REGISTER_RESPONSE";
    root_obj["method"] = "register";
    if (m_result == Status::SUCCESS) {
        args_obj["nickname"] = incomingItem.getNick();
        args_obj["ID"] = incomingItem.getID();
        args_obj["success"] = 1;
    } else {
        args_obj["success"] = 0;
    }

    root_obj.insert("args", args_obj);
    return QJsonDocument(root_obj).toJson();
}

QByteArray ServerResolver::prepareLoginResponse() {
    QJsonObject root_obj;
    QJsonObject args_obj;

    root_obj["messageType"] = "LOGIN_RESPONSE";
    root_obj["method"] = "login";
    args_obj["success"] = static_cast<int>(m_stateIsLogin);
    root_obj.insert("args", args_obj);

    return QJsonDocument(root_obj).toJson();
}
