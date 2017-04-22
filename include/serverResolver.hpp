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
        //todo: add comments what's purpose of these atributes
        QString         m_path;                 //path to folder where are saved all databases ?
        int             m_result;
        Item            m_item;                 // incoming request from client
        bool            m_stateIsLogin;
        bool            m_stateIsForward;
        User            m_user;                 // info about user, from database
        QMutex *const   m_mutexFile;
        QString         m_fileName;             //filename of what?

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

     public:
        //Constructors
        /**
         * Construct a resolver object.
         * @param path The path to the directory which should contain database file.
         * @param file The file name which shall contain the DB.
         * @param mutexFile Lock of the file to prevent data race.
         */
        ServerResolver(const QString &path, const QString &file, QMutex *const mutexFile = nullptr);


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