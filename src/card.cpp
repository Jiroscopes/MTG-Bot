#include "card.h"

card::card(std::string inputName) 
{
	this->givenName = inputName;
}

Document card::findCard(std::string& cardName)
{
	auto r = cpr::Get(cpr::Url{ "https://api.scryfall.com/cards/named" },
		cpr::Parameters{ { "fuzzy", cardName } }); //fuzzy allows non-exact searches
	r.status_code;                  // 200
	r.header["content-type"];       // application/json; charset=utf-8

	return parseCard(r.text);
}


Document card::parseCard(std::string& rawJSON)
{
	const char* bruh = rawJSON.c_str();
	Document incomingData;
	incomingData.Parse(bruh);
	
	return incomingData;
}

std::string card::extractImage(Document& card)
{
	// FindMember performs HasMember() and returns a pointer to the value if found
	auto imageURI = card.FindMember("image_uris");

	if (imageURI != card.MemberEnd())
	{
		auto imageURL = card["image_uris"].FindMember("large");
		if (imageURL != card.MemberEnd())
		{
			return imageURL->value.GetString(); // the arrow accesses a struct through a pointer?
		}
	}
	return  "No card found";
}

std::vector<std::string> card::findInfo(std::string infoArr[], int length, Document& cardJSON)
{
	std::vector<std::string> returnedInfo(4);
	for (int i = 0; i < length; i++)
	{
		// convert the string to const char* for the rapidJSON features
		const char* propName = infoArr[i].c_str(); // "image_uris", "cmc", "related_uris", "purchase_uris"

		auto findProp = cardJSON.FindMember(propName);

		if (findProp != cardJSON.MemberEnd())
		{
			// if it is the image links
			if (strcmp(propName, "image_uris") == 0)
			{
				// if has large
				auto imageURL = cardJSON[propName].FindMember("large");
				if (imageURL != cardJSON.MemberEnd())
				{
					returnedInfo[i] = imageURL->value.GetString();
					//cout << returnedInfo[i] << endl;
					continue;
				}
				// TODO handle image failure
				else {
					std::cout << "no image" << std::endl;
				}
			}
			// if it has related_uris
			if (strcmp(propName, "related_uris") == 0)
			{
				//if it has edhrec
				auto edhrecURL = cardJSON[propName].FindMember("edhrec");
				if (edhrecURL != cardJSON.MemberEnd()) {
					returnedInfo[i] = edhrecURL->value.GetString();
					//cout << returnedInfo[i] << endl;
					continue;
				}
			}
			// if it is the purchase_uris
			if (strcmp(propName, "purchase_uris") == 0)
			{
				//if it has tcgplayer
				auto tcgPlayerURL = cardJSON[propName].FindMember("tcgplayer");
				if (tcgPlayerURL != cardJSON.MemberEnd())
				{
					returnedInfo[i] = tcgPlayerURL->value.GetString();
					//cout << returnedInfo[i] << endl;
					continue;
				}
			}
			// else get cmc
			//returnedInfo[i] = findProp->value.GetDouble();
			char cmc = std::to_string(findProp->value.GetFloat())[0]; // data returns 3.000000, so just trim to 1 char
			returnedInfo[i] = cmc;
			//cout << cmc  << endl;
		}
	}

	return returnedInfo;
}


std::string card::getCardName(Document& card)
{
	std::string cardName = card.FindMember("name")->value.GetString();

	return cardName;
}
