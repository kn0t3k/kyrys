#pragma once

namespace Kyrys {
	namespace Enums {
		namespace Defaults {
			enum DefaultPortNumber {
				DEFAULT = 14251			//default port at which both client and server run
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
			namespace Registration{
				enum Status {
					SUCCESS = 0,	   		//Registration process was finished succesfully
					REGISTRATION_STARTED, 	//Registration has started
					BAD_PASSWORD,	   	  	//User didn't repeat password correctly
					CREDENTIALS_LOADED,   	//User wrote nick and password correctly
					PASSWORD_HASHED,		//Password was succesfully hashed
					MODIFIED_NICKNAME,    	//User wrote nickname which is already registered, so server changed it to similar and unique form
					SERVER_ERROR	   		//Uknown error on server's side
				};
				enum PasswordSecQuality {
					GOOD = 0,				//Good password which fullfill all security requirements
					TOO_SHORT				//Too short password less than 8 chars long
				};
			}	
			namespace Login{
				enum Status{
					SUCCESS = 0,	 	//Login process was finished succesfully
					LOGIN_STARTED,	 	//Login has started
					CREDENTIALS_LOADED, //User wrote nick and password
					PASSWORD_HASHED,	//Password was succesfully hashed
					ABSENT_NICKNAME, 	//User used nickname which doesn't exist in server's database
					BAD_PASSWORD,	 	//User wrote bad password acording to his nickname
					SERVER_ERROR,    	//Uknown error on server's side

				};
			}
		}
		namespace Item {
			enum MethodType {
				REGISTER,
				LOGIN,
				INVALID_CMND,
				UNKNOWN
			};
		}
		namespace JsonMessage {
			enum MessageType {
				REGISTER_REQUEST,
				REGISTER_RESPONSE,
				LOGIN_REQUEST,
				LOGIN_RESPONSE
			};
		}
	}
}