#include "mtgbot.h";

void mtgbot::onMessage(SleepyDiscord::Message message) 
{
	std::string msg = message.content;
	std::size_t startPos;

	if ((startPos = msg.find("[")) != std::string::npos)			//string::npos is the not matched value ~ basically
	{
		std::size_t endPos;
		if ((endPos = msg.find("]")) != std::string::npos && endPos != startPos + 1) 
		{
			std::string inputCardName =								// The input from the user
				msg.substr(startPos + 1, endPos - startPos - 1);	
			card mtgCard(inputCardName);							// Create the card
			auto cardJSON = mtgCard.findCard(inputCardName);		// The cards JSON
			std::string arr[4] =									// The info for the card to display
			{ 
				"image_uris", 
				"cmc", 
				"related_uris", 
				"purchase_uris" 
			};
																	// TO-DO: Refactor this, too messy
			std::vector<std::string> embedArr =						// The array full of info about mtgCard
				mtgCard.findInfo(arr, 4, cardJSON);

			
			SleepyDiscord::Embed embed;								// build Embed
			embed.title = mtgCard.getCardName(cardJSON);
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
