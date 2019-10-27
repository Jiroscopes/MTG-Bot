#include "mtgbot.h"

using namespace rapidjson;
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
				//string* namePtr = &cardName; // namePtr = address of cardName; *namePtr = value
				auto cardImage = findCard(cardName);
				// build Embed
				SleepyDiscord::Embed embed;
				embed.title = cardName;
				embed.description = "Description";
				embed.url = "https://somewhere.someplace";
				embed.footer.text = "Footer Text";
				embed.image.url = cardImage;
				// Send
				sendMessage(message.channelID, "", embed);

			}
		}
	}
	/* TO DO: findCard makes a request for the card information, then passes the data back.
		Card Information:
			-Name
			-Link to tcgplayer
			-Link to edhrec
	*/
	string findCard(string& cardName) {
		auto r = cpr::Get(cpr::Url{ "https://api.scryfall.com/cards/named" },
			cpr::Parameters{ { "fuzzy", cardName } }); //fuzzy allows non-exact searches
		r.status_code;                  // 200
		r.header["content-type"];       // application/json; charset=utf-8
		
		return extractImage(r.text);
	}

	string extractImage(string data) {
		const char* bruh = data.c_str();
		Document incomingData;
		incomingData.Parse(bruh);
		// FindMember performs HasMember() and returns a pointer to the value if found
		auto imageURI = incomingData.FindMember("image_uris");
		if (imageURI != incomingData.MemberEnd()) {
			auto imageURL = incomingData["image_uris"].FindMember("large");
			if (imageURL != incomingData.MemberEnd()) {
				return imageURL->value.GetString(); // the arrow accesses a struct through a pointer?
			}
		}
		return  "Fail";
	}
};

int main() {
	MyClientClass client("NjM0MTg4NzczMDc2MjM4MzU4.XayN_w.mvyN5REvXXWrwqL8VtEnoGjGh0k", 2);
	client.run();
}
