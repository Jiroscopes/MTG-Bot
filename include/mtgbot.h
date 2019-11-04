#pragma once
#include <cpr/cpr.h>
#include "card.h"
#include "sleepy_discord/websocketpp_websocket.h"

class mtgbot : public SleepyDiscord::DiscordClient 
{
public:
	using SleepyDiscord::DiscordClient::DiscordClient;

	void onMessage(SleepyDiscord::Message message);

};
