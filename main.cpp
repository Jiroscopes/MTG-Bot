
#include <iostream>
#include "deps/sleepy-discord/include/sleepy_discord/websocketpp_websocket.h"
#include <string>
#include <cpr/cpr.h>

class MyClientClass : public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;

	//enum Command : unsigned char {
	//	HELP = 0
	//};

	//Command userCommand;
	void onMessage(SleepyDiscord::Message message) override {
		std::string msg = message.content;
		std::size_t startPos;

		//std::string::npos is the not matched value ~ basically
		if ((startPos = msg.find("[")) != std::string::npos) {
			std::size_t endPos;
			if ((endPos = msg.find("]")) != std::string::npos && endPos != startPos + 1) {
				std::string cardName = msg.substr(startPos + 1, endPos - startPos - 1);

				sendMessage(message.channelID, findCard(cardName));
			}
		}
	}

	std::string findCard(std::string cardName) {
		auto r = cpr::Get(cpr::Url{ "https://api.scryfall.com/cards/named" },
			//cpr::Authentication{ "jiroscopes", "16Popste" },
			cpr::Parameters{ { "exact", "mana drain" },{ "format", "image" } });
		r.status_code;                  // 200
		r.header["content-type"];       // application/json; charset=utf-8
										//return r.text;
		std::cout << r.text;
		return r.text;
	}
};

int main() {
	MyClientClass client("NjM0MTg4NzczMDc2MjM4MzU4.Xafanw.0m77Mx8rPRFw5MKn4H9cOfSlDl0", 2);
	client.run();
}