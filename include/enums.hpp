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