#pragma once
#include "mqtt/async_client.h"
namespace globals {
	extern bool record;
	extern std::string filename;
	extern mqtt::async_client *client;
}
