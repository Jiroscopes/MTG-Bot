
#include <iostream>
#include <algorithm>
#include <string>
#include <cpr/cpr.h>
#include "deps/sleepy-discord/include/sleepy_discord/websocketpp_websocket.h"
#include "deps/json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

class MyClientClass : public SleepyDiscord::DiscordClient {
public:

	using SleepyDiscord::DiscordClient::DiscordClient;

	void onMessage(SleepyDiscord::Message message) override {
		string msg = message.content;
		std::size_t startPos;

		//string::npos is the not matched value ~ basically
		if ((startPos = msg.find("[")) != string::npos) {
			std::size_t endPos;
			if ((endPos = msg.find("]")) != string::npos && endPos != startPos + 1) {
				string cardName = msg.substr(startPos + 1, endPos - startPos - 1);
				string cardImage = findCard(cardName);
				std::cout << cardImage << std::endl;
				json embed;
				embed = {
					{"image", cardImage}
				};
				sendMessage(message.channelID, embed);
			}
		}
	}

	string findCard(string cardName) {
		auto r = cpr::Get(cpr::Url{ "https://api.scryfall.com/cards/named" },
			cpr::Parameters{ { "exact", cardName } });
		r.status_code;                  // 200
		r.header["content-type"];       // application/json; charset=utf-8
	
		return extractImage(r.text);;
	}

	string extractImage(string data) {
		json j_string = data;

		json parsed = json::parse(data);
		string imageLink = parsed["image_uris"]["large"].dump();
		imageLink.erase(std::remove(imageLink.begin(), imageLink.end(), '\"'), imageLink.end());
		return imageLink;
	}
};

int main() {
	MyClientClass client("NjM0MTg4NzczMDc2MjM4MzU4.XayN_w.mvyN5REvXXWrwqL8VtEnoGjGh0k", 2);
	client.run();



}