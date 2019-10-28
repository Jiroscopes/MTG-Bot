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
				string arr[4] = {"image_uris", "cmc", "related_uris", "purchase_uris"};
				
				vector<string> embedArr = findInfo(arr, 4, cardImage);

				// build Embed
				SleepyDiscord::Embed embed;
				embed.title = cardName;
				embed.image.url = embedArr[0];

				// Send
				sendMessage(message.channelID, "", embed);

				// cleanup the array we received
				/*delete [] embedArr;*/
			}
		}
	}
	/* TO DO: findCard makes a request for the card information, then passes the data back.
		Card Information:
			-Name
			-Link to tcgplayer
			-Link to edhrec
	*/

	// returns the card information
	Document findCard(string& cardName) {
		auto r = cpr::Get(cpr::Url{ "https://api.scryfall.com/cards/named" },
			cpr::Parameters{ { "fuzzy", cardName } }); //fuzzy allows non-exact searches
		r.status_code;                  // 200
		r.header["content-type"];       // application/json; charset=utf-8
		
		return parseCard(r.text);
	}

	// Parses JSON string and returns the object(Document)
	Document parseCard(string& rawJSON) {
		const char* bruh = rawJSON.c_str();
		Document incomingData;
		incomingData.Parse(bruh);

		return incomingData;
	}

	string extractImage(Document& card) {
		// FindMember performs HasMember() and returns a pointer to the value if found
		auto imageURI = card.FindMember("image_uris");

		if (imageURI != card.MemberEnd()) {
			auto imageURL = card["image_uris"].FindMember("large");
			if (imageURL != card.MemberEnd()) {
				return imageURL->value.GetString(); // the arrow accesses a struct through a pointer?
			}
		}
		return  "No card found";
	}

	// take in array of info that you want, and return a ready embed
	vector<string> findInfo(string infoArr[], int length, Document& cardJSON) {
		vector<string> returnedInfo(4);
		for (int i = 0; i < length; i++) {
			// convert the string to const char* for the rapidJSON features
			const char* propName = infoArr[i].c_str(); // "image_uris", "cmc", "related_uris", "purchase_uris"
			
			auto findProp = cardJSON.FindMember(propName);

			if (findProp != cardJSON.MemberEnd()) {
				// if it is the image links
				if (strcmp(propName, "image_uris") == 0) {
					// if has large
					auto imageURL = cardJSON[propName].FindMember("large");
					if (imageURL != cardJSON.MemberEnd()) {
						returnedInfo[i] = imageURL->value.GetString();
						//cout << returnedInfo[i] << endl;
						continue;
					} 
					// TODO handle image failure
					else {
						cout << "no image" << endl;
					}
				}
				// if it has related_uris
				if (strcmp(propName, "related_uris") == 0) {
					//if it has edhrec
					auto edhrecURL = cardJSON[propName].FindMember("edhrec");
					if (edhrecURL != cardJSON.MemberEnd()) {
						returnedInfo[i] = edhrecURL->value.GetString();
						//cout << returnedInfo[i] << endl;
						continue;
					}
				}
				// if it is the purchase_uris
				if (strcmp(propName, "purchase_uris") == 0) {
					//if it has tcgplayer
					auto tcgPlayerURL = cardJSON[propName].FindMember("tcgplayer");
					if (tcgPlayerURL != cardJSON.MemberEnd()) {
						returnedInfo[i] = tcgPlayerURL->value.GetString();
						//cout << returnedInfo[i] << endl;
						continue;
					}
				}
				// else get cmc
				//returnedInfo[i] = findProp->value.GetDouble();
				char cmc = to_string(findProp->value.GetFloat())[0]; // data returns 3.000000, so just trim to 1 char
				returnedInfo[i] = cmc;
				//cout << cmc  << endl;
			}
		}

		return returnedInfo;
	}
};

int main() {
	MyClientClass client("NjM0MTg4NzczMDc2MjM4MzU4.XayN_w.mvyN5REvXXWrwqL8VtEnoGjGh0k", 2);
	client.run();
}
