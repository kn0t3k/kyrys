#pragma once

namespace Kyrys {
	namespace Enums {
		namespace Resolver {
			enum Status {
				UNKNOWN_METHOD,
				EMPTY_METHOD,				
				INVALID_JSON,
				FAILED,
				INVALID_ARGS,
				INVALID_CMND,
				SUCCESS
			};

			enum Mode {
				USE_JSON,
				INVALID
			};
		}
		namespace Client { //todo vymysli stavy
			namespace Registration{
				enum Status {
					RESERVED_NICKNAME, //User wrote nickname which is already registered
					BAD_PASSWORD,	   //User didn't repeat password correctly
					SERVER_ERROR,	   //Uknown error on server's side
					SUCCESS 		   //Registration process was finished succesfully
				};
			}	
			namespace Login{
				enum Status{
					ABSENT_NICKNAME, //User used nickname which doesn't exist in server's database
					BAD_PASSWORD,	 //User wrote bad password acording to his nickname
					SERVER_ERROR,    //Uknown error on server's side
					SUCCESS			 //Login process was finished succesfully 
				};
			}
		}

		namespace Server {
			enum Type {
				SSL,
				NO_SSL
			};
		}
		namespace Item {
			enum MethodType {
				REGISTER,
				CALL,
				INVALID_CMND,
				UNKNOWN
			};
		}
	}
}