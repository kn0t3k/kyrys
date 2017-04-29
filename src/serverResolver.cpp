#include <reference.hpp>
#include <serverResolver.hpp>

using Kyrys::ServerResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;

//Constructors
ServerResolver::ServerResolver(QMutex *const mutexFile)
        : m_item(),
          m_user("", ""),
          m_mutexFile(mutexFile) {
    clear();

    QString databasePath = QString(DATABASE_DIRECTORY) + "/" + QString(DATABASE_FILENAME);

    if (!QDir(DATABASE_DIRECTORY).exists()) {
        QDir().mkdir(DATABASE_DIRECTORY);
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);
    if (db.open()) {
        qDebug() << "db opened successfully";
    } else {
        qDebug() << "db failed to open at " + databasePath;
        return;
    }

    if (!db.tables().contains(QLatin1String("users"))) {
        qDebug() << "need to create a table";
        QSqlQuery query;
        query.prepare("CREATE TABLE users(ids integer primary key, nickname text)");
        if (!query.exec()) {
            qDebug() << "craete table error:  " << query.lastError();
            return;
        }
    } else {
        qDebug() << "table users already created";
    }

    if (!db.tables().contains(QLatin1String("passwords"))) {
        qDebug() << "need to create a table";
        QSqlQuery query;
        query.prepare("CREATE TABLE passwords(ids integer primary key, password text)");
        if (!query.exec()) {
            qDebug() << "craete table error:  " << query.lastError();
            return;
        }
    } else {
        qDebug() << "table passwords already created";
    }

}

//Getters
bool ServerResolver::isForward() { return m_stateIsForward; }

bool ServerResolver::isLogin() { return m_stateIsLogin; }

const Item &ServerResolver::getItem() const { return m_item; }

//Other methods
int ServerResolver::execute() {
    m_stateIsForward = false;
    m_stateIsLogin = false;

    int s = m_item.isValid();
    if (s != Status::SUCCESS) {
        return s;
    }

    switch (m_item.getMethodType()) {
        case MethodType::REGISTER : {
            return registerUser();
        }
        case MethodType::LOGIN : {
            return loginUser();
        }
        case MethodType::UNKNOWN : {
            return Status::UNKNOWN_METHOD;
        }
        case MethodType::FORWARD : {
            m_IDofRecipient = getUserID(m_item.getRecepient());
            if (m_IDofRecipient != -1) {
                m_stateIsForward = true;
                return Status::SUCCESS;
            } else {
                return Status::INVALID_CMND;
            }
        }
        default: {
            return Status::INVALID_CMND;
        }
    }
}

int ServerResolver::parse(const QString &data, Mode m) {
    clear();
    if (m == Mode::USE_JSON) {
        QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());

        if (d.isNull())    // fail - invalid JSON
            return Status::INVALID_JSON;

        QJsonObject object = d.object();

        m_item.clear();
        m_item.parse(object);

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
        query.bindValue(":nickname", m_item.getNick());
        if (!query.exec()) {
            qDebug() << "select error:  " << query.lastError();
            return Status::FAILED;
        } else {
            if (query.next()) {
                m_item.increaseNick();
            } else {
                break;
            }
        }
    }

    //insert the user into database
    query.prepare("INSERT INTO users (nickname) VALUES (:nickname)");
    query.bindValue(":nickname", m_item.getNick());
    if (!query.exec()) {
        qDebug() << "insert error:  " << query.lastError();
        return Status::FAILED;
    }

    //insert the user's password into database
    query.prepare("INSERT INTO passwords (password) VALUES (:password)");
    query.bindValue(":password", m_item.getPasswordHash());
    if (!query.exec()) {
        qDebug() << "insert error:  " << query.lastError();
        return Status::FAILED;
    }

    return Status::SUCCESS;
}

QByteArray ServerResolver::prepareResponse() {
    QJsonObject root_obj;
    QJsonObject args_obj;
    QJsonDocument document;

    switch (m_item.getMethodType()) {
        case MethodType::REGISTER : {
            root_obj["messageType"] = "REGISTER_RESPONSE";
            root_obj["method"] = "register";
            if (m_result == Status::SUCCESS) {
                args_obj["nickname"] = m_item.getNick();
                args_obj["ID"] = m_item.getID();
                args_obj["success"] = 1;
            } else {
                args_obj["success"] = 0;
            }

            root_obj.insert("args", args_obj);
            break;
        }
        case MethodType::LOGIN : {
            root_obj["messageType"] = "LOGIN_RESPONSE";
            root_obj["method"] = "login";
            args_obj["success"] = static_cast<int>(m_stateIsLogin);
            root_obj.insert("args", args_obj);
            break;
        }
        case MethodType::FORWARD : {
            root_obj["method"] = "forward";
            root_obj["args"] = m_item.getArgs();
            break;
        }
        default : {
            break;
        }
    }

    return QJsonDocument(root_obj).toJson();
}

int ServerResolver::getUserID(const QString &nickName) {
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
    m_item = Item();
    m_stateIsForward = false;
    m_stateIsLogin = false;
    m_user = User("", "");
    m_IDofRecipient = -1;
}

int ServerResolver::loginUser() {
    // get user ID from db, search by nick, set this ID to m_item
    int ID = getUserID(m_item.getNick());
    if (ID == -1) {
        return Status::INVALID_CRED;
    } else {
        getUserPassword(ID);
        // compare received hash with the one from database
        if (m_item.getPasswordHash() == QString(m_user.getPasswordHash())) {
            m_item.setID(ID);
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
            m_user.setPasswordHash(pass.toLocal8Bit());
        }
    }
}
