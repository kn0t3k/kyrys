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
        Item m_item;                 // incoming request from client
        bool m_stateIsLogin;    // if resolver is in state of logging a user in
        bool m_stateIsForward;  // if resolver is in state of forwarding a message
        User m_user;                 // info about user, from database
        QMutex *const m_mutexFile;      // mutex to protect file from data race
        QSqlDatabase db;

        //Getters
        int getUserID(const QString &nickName);

        //Other methods
        int execute();

        int registerUser();

        /**
         * @brief   Clear sets class to default state and resets all atributes to default value
         */
        void clear();

        int loginUser();

        void getUserPassword(int userID);

    public:
        //Constructors
        /**
         * Construct a resolver object.
         * @param path The path to the directory which should contain database file.
         * @param file The file name which shall contain the DB.
         * @param mutexFile Lock of the file to prevent data race.
         */
        ServerResolver(QMutex *const mutexFile = nullptr);


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