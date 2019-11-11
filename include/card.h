#pragma once
#include <iostream>
#include <algorithm>
#include "sleepy_discord/websocketpp_websocket.h"

using namespace rapidjson;
struct cardInfo
{
	int cmc;
	std::string name;
	std::string tcgLink; //tcgplayer link
	std::string edhRecLink;
	std::string imageLink;
};

class card
{
private:
	struct cardInfo currentCard;
	std::string givenName; // name given from message
public:
	
	card(std::string inputName);
	// returns the card information
	Document findCard(std::string& cardName);
	// Parses JSON string and returns the object(Document)
	Document parseCard(std::string& rawJSON);
	// returns image link
	std::string extractImage(Document& card);
	// take in array of info that you want, and return a ready embed
	std::vector<std::string> findInfo(std::string infoArr[], int length, Document& cardJSON);
	//get full name of card
	std::string getCardName(Document& card);
};

