#include <reference.hpp>
#include <serverResolver.hpp>

using Kyrys::ServerResolver;

using Kyrys::Enums::Resolver::Status;
using Kyrys::Enums::Item::MethodType;
using Kyrys::Enums::Resolver::Mode;
using Kyrys::Item;

//Constructors
ServerResolver::ServerResolver(const QString &path,
                               const QString &file,
                               QMutex *const mutexFile)
        : m_path(path),
          m_item(),
          m_user("", "",
                 QCryptographicHash::Sha3_512),
          m_mutexFile(
                  mutexFile),
          m_fileName(file) {
    m_result = -1;
    m_IDofRecipient = -1;
    m_stateIsLogin = false;
    m_stateIsForward = false;
}

//Getters
bool ServerResolver::isForward() { return m_stateIsForward; }

bool ServerResolver::isLogin() { return m_stateIsLogin; }

const Item &ServerResolver::getItem() const { return m_item; }


//Other methods
int ServerResolver::execute() {
    m_stateIsForward = false;
    m_stateIsLogin = false;

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

        m_item = Item(object);

        m_result = execute();

        return m_result;
    } else {
        return Status::FAILED;
    }
}

int ServerResolver::registerUser() {
    int s = m_item.isValid();
    if (s != Status::SUCCESS) {
        return s;
    }

    if (m_mutexFile != nullptr) {
        m_mutexFile->lock();
    }

    int status = Status::FAILED;

    QFile filePath(m_path + m_fileName);
    if (filePath.open(QIODevice::ReadOnly)) {
        QTextStream fileStream(&filePath);
        int ID = 0;
        while (!fileStream.atEnd()) {
            auto line = fileStream.readLine();
            while (line.contains(m_item.getNick())) {
                m_item.increaseNick();
            }
            ID++;
        }
        m_item.setID(ID);

        filePath.close();

        if (filePath.open(QIODevice::WriteOnly | QIODevice::Append)) {
            filePath.write(m_item.serialize(ID).c_str());
            filePath.close();
            status = Status::SUCCESS;
        } else {
            status = Status::FAILED;
        }
    } else {
        qDebug() << "Failed to open database file";
    }

    if (m_mutexFile != nullptr) {
        m_mutexFile->unlock();
    }
    return status;
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

    qDebug() << QJsonDocument(root_obj).toJson();
    return QJsonDocument(root_obj).toJson();
}

int ServerResolver::getUserID(const QString &nickName) {
    if (m_mutexFile != nullptr) {
        m_mutexFile->lock();
    }

    int ID = -1;

    QFile filePath(m_path + m_fileName);
    if (filePath.open(QIODevice::ReadOnly)) {
        QTextStream fileStream(&filePath);

        while (!fileStream.atEnd()) {
            auto line = fileStream.readLine();
            if (line.contains(QString(";") + nickName + ";")) {
                QStringList args = line.split(";");
                bool isInt;
                ID = args.value(0).toInt(&isInt);

                if (!isInt) {
                    ID = -1;
                }

                m_user.setPasswordHash(args.value(3).toLatin1());

                break;
            }
        }

        filePath.close();
    } else {
        qDebug() << "Failed to open database file: " << filePath.fileName();
    }

    if (m_mutexFile != nullptr) {
        m_mutexFile->unlock();
    }
    return ID;
}

void ServerResolver::clear() {
    m_result = Status::FAILED;
    m_item = Item();
    m_stateIsForward = false;
    m_stateIsLogin = false;
    m_user = User("", "", QCryptographicHash::Sha3_512);
}

int ServerResolver::loginUser() {
    int isItemValid = m_item.isValid();
    if (isItemValid != Status::SUCCESS) {
        return isItemValid;
    } else {
        // get user ID from db, search by nick, set this ID to m_item
        int ID = getUserID(m_item.getNick());
        if (ID == -1) {
            return Status::INVALID_CRED;
        } else {
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
}

int ServerResolver::getRecipientID() const {
    return m_IDofRecipient;
}
