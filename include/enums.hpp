#pragma once

#define HASH QCryptographicHash::Sha3_512 //Default Hashing algorithm for

#define DEBUG 0

#define DATABASE_FILENAME "users.db"
#define PASSWORDS_FILENAME "passwords"
#define DATABASE_DIRECTORY "database"

namespace Kyrys {
    namespace Enums {
        namespace Defaults {
            enum DefaultPortNumber {
                DEFAULT_PORT = 14251            //default port at which both client and server run
            };
        }
        namespace Resolver {
            enum Status {
                UNKNOWN_METHOD,
                EMPTY_METHOD,
                INVALID_JSON,
                FAILED,
                INVALID_ARGS,
                INVALID_CMND,
                INVALID_CRED,
                SUCCESS
            };

            enum Mode {
                USE_JSON,
                INVALID
            };
        }
        namespace Client {
            namespace Registration {
                enum Status {
                    SUCCESS = 0,            //Registration process was finished succesfully
                    REGISTRATION_STARTED,    //Registration has started
                    BAD_PASSWORD,            //User didn't repeat password correctly
                    CREDENTIALS_LOADED,        //User wrote nick and password correctly
                    PASSWORD_HASHED,        //Password was succesfully hashed
                    REQUEST_ERROR,            //Registration was unable to send REGISTER_REQUEST message to server
                    RESPONSE_ERROR,            //Registration was unable to receive REGISTER_RESPONSE message from server
                    MODIFIED_NICKNAME,      //User wrote nickname which is already registered, so server changed it to similar and unique form
                    SERVER_ERROR            //Uknown error on server's side
                };
                enum PasswordSecQuality {
                    GOOD = 0,                //Good password which fullfill all security requirements
                    TOO_SHORT                //Too short password less than 8 chars long
                };
            }
            namespace Login {
                enum Status {
                    SUCCESS = 0,        //Login process was finished succesfully
                    FAIL,
                    LOGIN_STARTED,        //Login has started
                    CREDENTIALS_LOADED, //User wrote nick and password
                    PASSWORD_HASHED,    //Password was succesfully hashed
                    REQUEST_ERROR,
                    RESPONSE_ERROR,
                    ABSENT_NICKNAME,    //User used nickname which doesn't exist in server's database
                    BAD_PASSWORD,        //User wrote bad password acording to his nickname
                    SERVER_ERROR,        //Uknown error on server's side

                };
            }
        }
        namespace Item {
            enum MethodType {
                REGISTER,
                LOGIN,
                FORWARD,
                INVALID_CMND,
                UNKNOWN
            };
        }
        namespace JsonMessage {
            enum MessageType {
                REGISTER_REQUEST,
                REGISTER_RESPONSE,
                LOGIN_REQUEST,
                LOGIN_RESPONSE,
                FORWARD,
                CHAT_REQUEST,
                CHAT_RESPONSE,
                UNKNOWN
            };
        }
        namespace Chat {
            enum Accessibility {
                ONLINE,         //User will get notice about incoming chat
                OFFLINE,        //User's chat will automatically send back refusing CHAT_RESPONSE message
                CHATING,        //This status is active during some chat and inform sender about this status, also program will create history of tries and inform receiver about missed CHAT_REQUESTs
            };
            enum DataEncryption {
                PLAIN_TEXT,         //Data part of message between 2 users will be plaintext
                SHARED_KEY          //                                        -||- encrypted by symetric cipher by sharedkey
            };
        }
    }
}