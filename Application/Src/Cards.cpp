#include "Cards.h"
#include <random>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#define CARDS 32
void Card_System::load_assets()
{
	for (int i = 1; i <= CARDS; i++)
	{
		Empaerior::string path = "assets/card";
		path += std::to_string(i);
		path += ".png";
		std::cout << path << '\n';
		id_to_path.emplace(i - 1,path);
	}



}
#undef CARDS



void Card_System::load_board(Empaerior::ECS& ecs)
{
	//generate cards
	APP_INFO("Randomizing cards");
	std::random_device rd;
	std::mt19937 rng(rd());
	
	std::vector<int> id;

	for (int i = 0; i < 32; i++)
	{
		id.push_back(i);
		id.push_back(i);

	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(id.begin(), id.end(), std::default_random_engine(seed));
	


	


	APP_INFO("Generating board");
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//create entity
			board[i][j] = ecs.create_entity_ID();
			ecs.add_component<Card_component>(board[i][j], { Empaerior::byte(id[i * 7 + j])});
		}
		
	}










}

void Card_System::Reveal(Empaerior::ECS& ecs, std::shared_ptr<Sprite_System>& spr_system,std::shared_ptr<T_E_System>& f_system,const Empaerior::u_inter& id, Empaerior::u_inter x, Empaerior::u_inter y)
{
	if (first_reveal)
	{
		spr_system->remove_sprite(ecs, id, 1);
		revealed_id = board[y][x];
		revealed_i = y;
		revealed_j = x;
		first_reveal = false;
	}
	else
	{
		std::cout << ecs.get_component<Card_component>(revealed_id).type << ' ' << ecs.get_component<Card_component>(board[y][x]).type << '\n';
		if (ecs.get_component<Card_component>(revealed_id).type == ecs.get_component<Card_component>(board[y][x]).type) { spr_system->remove_sprite(ecs, id, 1); }
		else
		{
			spr_system->remove_sprite(ecs, id, 1);
			f_system->add_function(ecs, board[y][x], 500, [&Ecs = ecs,&Spr_system = spr_system , id = board[y][x], i = y, j = x, r_id = revealed_id, r_i = revealed_i, r_j = revealed_j]()
				{
					Spr_system->add_sprite(Ecs, id, { 224 + 64 * int(i),96 * int(j),64,96 }, { 0,0,140,190 }, "assets/card_back.png", 1);
					Spr_system->add_sprite(Ecs, r_id, { 224 + 64 * int(r_i),96 * int(r_j),64,96 }, { 0,0,140,190 }, "assets/card_back.png", 1);
				});
		}
		first_reveal = true;
	
	}


}

