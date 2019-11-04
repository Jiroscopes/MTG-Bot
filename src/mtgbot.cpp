#include "mtgbot.h";

void mtgbot::onMessage(SleepyDiscord::Message message) 
{
	std::string msg = message.content;
	std::size_t startPos;

	//string::npos is the not matched value ~ basically
	if ((startPos = msg.find("[")) != std::string::npos) 
	{
		std::size_t endPos;
		if ((endPos = msg.find("]")) != std::string::npos && endPos != startPos + 1) 
		{

			std::string cardName = msg.substr(startPos + 1, endPos - startPos - 1);
			//string* namePtr = &cardName; // namePtr = address of cardName; *namePtr = value

			card someCard(cardName);
			auto cardImage = someCard.findCard(cardName);
			std::string arr[4] = 
			{ 
				"image_uris", 
				"cmc", 
				"related_uris", 
				"purchase_uris" 
			};

			std::vector<std::string> embedArr = someCard.findInfo(arr, 4, cardImage);

			// build Embed
			std::cout << cardName << std::endl;
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
