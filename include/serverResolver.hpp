#pragma once

#include <reference.hpp>
#include <item.hpp>
#include <user.hpp>

#include <QtNetwork/QSslSocket>
#include <QtCore/QMutex>

namespace Kyrys {
    class ServerResolver {
        typedef Kyrys::Enums::Item::MethodType MethodType;
        typedef Kyrys::Enums::Resolver::Mode Mode;
        typedef Kyrys::Item Item;
        typedef Kyrys::User User;

    private:
        int m_result;   // general result value to hold return values of various functions
        int m_IDofRecipient;    // id of user that the message shall be forwarded to
        Item incomingItem;                 // incoming request from client
        bool m_stateIsLogin;    // if resolver is in state of logging a user in
        bool m_stateIsForward;  // if resolver is in state of forwarding a message
        User m_userFromDB;                 // info about user, from database
        QSqlDatabase m_dbHandle;
        QString m_messageDataToForward;

        //Getters
        int getUserIDFromDB(const QString &nickName);

        void getUserPassword(int userID);

        //Other methods
        int execute();

        int registerUser();

        /**
         * @brief   Clear sets class to default state and resets all atributes to default value
         */
        void clear();

        int loginUser();

        int forwardChat();

        QByteArray prepareRegisterResponse();

        QByteArray prepareLoginResponse();

    public:
        //Constructors
        /**
         * Construct a resolver object.
         */
        ServerResolver();


        //Getters
        /**
         * Check if resolver is in state which shall forward a message.
         * @return True if forwarding, false otherwise.
         */
        bool isForward();

        /**
         * Check if resolver is in state which shall login a user.
         * @return Return true if login, false otherwise.
         */
        bool isLogin();

        const Item &getItem() const;

        int getRecipientID() const;

        //Other methods
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
         * Prepares JSON message in QByteArray, which contains corerect response for
         * a particular request - login response for login etc.
         *
         * @return QByteArray with the  response, can be empty of method type is not
         * supported.
         *
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
    };
}