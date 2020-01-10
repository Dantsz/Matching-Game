#pragma once
#include <Empaerior.h>

//the card's information
struct Card_component
{
	Empaerior::byte type;
};


class Card_System : public Empaerior::System
{
public:

	void load_assets();

	void load_board(Empaerior::ECS& ecs);

	
	
	//reveal a card
	void Reveal(Empaerior::ECS& ecs, const Empaerior::u_inter& id, Empaerior::u_inter x, Empaerior::u_inter y);


	//if a card is revealed
	bool isRevealed = false;
	uint64_t board[8][8];
	

	//if od the card to the path of the texture;
	std::unordered_map<Empaerior::u_inter, Empaerior::string> id_to_path;
};