#pragma once

#include <reference.hpp>
#include <item.hpp>
#include <user.hpp>
#include <QtNetwork/QSslSocket>
#include <QtCore/QMutex>

namespace Kyrys {
    class Resolver {
        typedef Kyrys::Enums::Item::MethodType MethodType;
        typedef Kyrys::Enums::Resolver::Mode Mode;
        typedef Kyrys::Item Item;
        typedef Kyrys::User User;

     private:
        QString m_path;

        int m_result;

        Item m_item;    // incoming request from client

        bool m_stateIsLogin;

        bool m_stateIsForward;

        User m_user;    // info about user, from database

        QMutex *const m_mutexFile;

        int execute();

        int registerUser();

        int getUserID(const QString &nickName);

        void clear();

        int loginUser();

     public:
        /**
         * @brief      Construct a resolver object.
         *
         * @param[in]  path  The path to the directory which should contain database file.
         */
        Resolver(const QString &path, QMutex *const mutexFile = nullptr);

        /**
         * @brief      Parse input string using specified mode.
         *
         * @param[in]  data  The data to parse.
         * @param[in]  m     The mode in which the resolver operates.
         *
         * @return     Return success if succeeded, fail otherwise.
         */
        int parse(const QString &data, Mode m);

        /**
         * Prepares JSON message in QByteArray, which containc corerct response for
         * a particular request - login response for login etc.
         * @return QByteArray with the  response, can be empty of method type is not
         * supported.
         * EXAMPLES:
         * register response:
         * {
         *      methodType : "REGISTER_RESPONSE",
         *      method : "register",
         *      args : {
         *              "nickname" : "XXX",
         *              "ID" : ID,
         *              "success" : 1 | 0   // true of false
         *              }
         *  }
         *
         *  login response:
         *  {
         *      methodType : "LOGIN_RESPONSE",
         *      method : "login",
         *      args : {
         *              "success" : 1 | 0   //true or false
         *              }
         *  }
         */
        QByteArray prepareResponse();

        /**
         * Check if resolver is in state which shall forward a message.
         * @return True if forwarding, false otherwise.
         */
        bool isForward() { return m_stateIsForward; }

        /**
         * Check if resolver is in state which shall login a user.
         * @return Return true if login, false otherwise.
         */
        bool isLogin() { return m_stateIsLogin; }

        const Item &getItem() const { return m_item; }
    };
}