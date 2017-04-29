#pragma once

#include <reference.hpp>
#include <user.hpp>
#include <istream>
#include <item.hpp>
#include <chat.hpp>

namespace Kyrys {
    class Client : public QObject {
    Q_OBJECT

        typedef Kyrys::User User;
        typedef std::string string;
        typedef Kyrys::Enums::JsonMessage::MessageType MessageType;
		typedef Kyrys::Chat Chat;

    private:
        QSslSocket *m_socket = nullptr;
        User m_user;
        QString m_hostname;
        quint16 m_port;
		//Chat m_chat;

    private slots:

        void socketEncrypted();

        void socketReadyRead();

        void sslErrors(const QList<QSslError> &errors);

        void socketError(QAbstractSocket::SocketError error);

        void copyRegistrationItem(const Item &item); //copy informations from Item to client and user

    public:
        //Constructors
        /**
        * @brief Client constructor.
        * @details Constructs a new client and prepares it to connect to a server.
        *
        * @param hostName Hostname of the server to connect to.
        * @param port Port at which the server listens.
        * @param parent QT parent object.
        */
        explicit Client(const QString &hostName, quint16 port, QObject *parent = 0);

        //Destructors
        /**
         * @brief Client destructor.
         * @details De-allocates the used memory.
         */
        ~Client();


        //Getters
        const User &getUser() const;

        /**
         * @brief Connect to the specified server.
         * @details Sets up a connection between a client and server, compares the server-supplied
         * certificate to the one distributed with the client to check server authority.
         * @return Return true if connection was succesfull.
         */
        bool secureConnect();

        /**
         * @brief Send specified data throught the socket.
         * @details Converts data from QString to char * and sends it.
         * @param data Input data.
         * @return Return true if succeeded, false otherwise.
         */
        bool sendData(const QString &data);


        //Loading Credentials for Login/Registration
        int loadRegistrationCredentials(string &nickname, string &password, std::istream &in = std::cin);

        int loadLoginCredentials(string &nickname, string &password, std::istream &in = std::cin);

        /**
         * @brief			This method controls if password meets the safety requiremets
         * @param password	Password whish will be controlled
         * @return			PasswordSecQuality::GOOD = 0 if password is ok different value otherwise, more info in enums.hpp
         */
        int controlPasswordSecQuality(const string &password) const;

        /**
         * @brief 	 Registration of new user
         * @warning  User's password has to be salted and hashed inside method. Password must not leave method in open format.
         * @return	 0 in case of succesfull registration, non-zero code otherwise
         * 			 code 1 = user was not able to type same password twice at least one time
         * @note	 Registered user is not automatically logged in. After registration you have to pass login procedure.
         */
        int registration(std::istream &in = std::cin); //not finished yet

        /**
         * @brief  Login of registered user
         * @return Same as registration
         * @note
         */
        int login(std::istream &in = std::cin); //not finished yet


        /**
         * @brief  This method creates json message, which send client to server when want to authenticate user. It is used in both cases, login and registration
         * @param messageType	write MessageType::LOGIN_CALL in case of login
         * 							  MessageType::REGISTER_CALL otherwise
         * @return message in format QJsonDocument
         * @note   message format examples:
         * 		   EXAMPLE 1 - Registration:
         * 		   {
         * 		   		"messageType": "REGISTER_REQUEST",
                        "method": "register",
                        "args" : {
                            "nickname": "nickname of user",
                            "password": "hash of user's password"
                           }
                   }

                   EXAMPLE 2 - login:
                   {
                            "messageType": "LOGIN_REQUEST",
                        "method": "login",
                        "args" : {
                            "nickname": "nickname of user",
                            "password": "hash of user's password"
                           }
                   }
         */
        QJsonDocument jsonMessageUserAuthentication(MessageType messageType);



        //QJsonDocument jsonCreateCallRequest(Friend recipient)

        //void createSharedKey();

        /**
         * @brief This method covers whole chat with friend. It covers whole process of chating with one friend defined by receiversID and it will keep
         *        encrypted end-to-end tunnel trough server until one of the participants finish communication.
         * @param receiverID[in]
         */
        //void callFriend(int recipientID);



        /**
         EXAMPLE:
        {
             "messageType": "UPDATE_REQUEST",
             "method": "update",
             "args" : {
                "IP adress": "IPv4 adress",
                "Port": "quint16 type"
            }
        }
         */
        //QJsonDocument jsonMessageUserUpdate(MessageType messageType);

        /**
         * @brief run	It's a method executing commands from Client commandline.
         * @param in 	In is parameter for pushing fake input instead of standard input from std::cin. Use this param for testing purpose. Default value is std::cin
         */
        void run(std::istream &in = std::cin);


        /**
         *	@brief send message to socket m_socket and control if data was send succesfully
         *	@note	expecting that socket is already connected to server, if not method will fail
         *	@return true if successfully send data, false otherwise
         */
        bool send(const QString &data);

        /**
         *	@brief receive message from socket m_socket and control if data was received succesfully
         *	@param [out]response	reference to buffer response where will be written data from socket
         *	@return true if successfully received data, false if no data arrived in set timeout 1000ms
         *	@note	expecting that socket is already connected to server, if not method will fail
         */
        bool receive(QByteArray &response);


		/**
		 * @brief This method will iniciate chat with some another user
		 * @param in
		 * @return
		 */
		int chat(std::istream &in = std::cin); //todo


		/**
		 * @brief Adds new user into friend list in user
		 * @return
		 */
		int addFriend(); //todo
    };
}
