#pragma once

namespace Kyrys {
	namespace Enums {
		namespace Resolver {
			enum Status {
				UNKNOWN_METHOD,
				EMPTY_METHOD,				
				INVALID_JSON,
				FAILED,
				SUCCESS
			};
			enum MethodType{
				REGISTER,
				CALL,
				UNKNOWN
			};
			enum Mode{
				USE_JSON
			};
		}
		namespace Server{
			enum Type{
				SSL,
				NO_SSL
			};
		}
	}
}