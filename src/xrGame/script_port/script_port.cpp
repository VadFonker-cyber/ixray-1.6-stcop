#include "_script_port_headers.h"

namespace scripts {

	void SemiLog(const std::string& Msg) 
	{
		Log(Msg.c_str());
	}

	long long time_global()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
	};

}

