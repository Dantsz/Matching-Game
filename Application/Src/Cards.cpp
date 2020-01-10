#include "Cards.h"
#include <random>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#define CARDS 32
void Card_System::load_assets()
{
	for (int i = 1; i <= CARDS; i++)
	{
		Empaerior::string path = "card";
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

