#include <Empaerior.h>
#include <main.h>
#include "Cards.h"
#include "Timer_Execution_Component.h"
//An example of what a application might look like


bool rect_contains_point(const SDL_Rect& rect, int x, int y)
{


	if (y <= rect.y || y > rect.y + rect.h) return false;
	if (x <= rect.x || x > rect.x + rect.w) return false;

	return true;
}

//a user defined state
class APP_State : public Empaerior::State
{

public:
	APP_State()
	{
		//backgorund




		//set camera
		camera = { 0,0,960,800 };
		
        ecs.Init();
		ecs.register_component<Empaerior::Position_Component>();
		ecs.register_component<Empaerior::Camera_Component>();
		ecs.register_component<Empaerior::Sprite_Component>();
		ecs.register_component<Empaerior::Event_Listener_Component>();
		ecs.register_component<Card_component>();
		ecs.register_component<T_E_Component>();

		spr_system = ecs.register_system <Sprite_System>();
		event_system = ecs.register_system<Event_System>();
		card_system = ecs.register_system<Card_System>();
		f_timed_system = ecs.register_system<T_E_System>();

		ecs.add_component_to_system<Empaerior::Sprite_Component, Sprite_System>();
		ecs.add_component_to_system<Empaerior::Event_Listener_Component, Event_System>();
		ecs.add_component_to_system<Card_component, Card_System>();

		card_system->load_assets();
		card_system->load_board(ecs);


		//add components to all cards 
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				//adds all components
				ecs.add_component<T_E_Component>(card_system->board[i][j], {});
				ecs.add_component<Empaerior::Sprite_Component>(card_system->board[i][j], {});
				ecs.add_component<Empaerior::Event_Listener_Component>(card_system->board[i][j], {});

				//add sprite
				
				spr_system->add_sprite(ecs, card_system->board[i][j], { 224 + 64 * i,96 * j,64,96 }, { 0,0,140,190 }, card_system->id_to_path[ecs.get_component<Card_component>(card_system->board[i][j]).type], 1);
				spr_system->add_sprite(ecs, card_system->board[i][j], { 224 + 64 * i,96 * j,64,96 }, { 0,0,140,190 }, "assets/card_back.png", 1);
				//add events on mouse down
				event_system->add_event_to_entity(ecs, card_system->board[i][j], SDL_MOUSEBUTTONDOWN, 
					[&Ecs = ecs ,&Spr_system = spr_system,&Card_system = card_system,&F_timed_system = f_timed_system,entity = card_system->board[i][j] , &kamera = camera, x = j, y = i](SDL_Event const& event)
					{
					auto mpos = Empaerior::get_world_mouse_coords(kamera);
					
						if (rect_contains_point(Ecs.get_component<Empaerior::Sprite_Component>(entity).sprites[0].get_dimensions(),mpos.first,mpos.second))
						{
							Card_system->Reveal(Ecs, Spr_system, F_timed_system, Card_system->board[y][x], x, y);
						}
					}
				);

			}

		}


		


	}

	void Update(const Empaerior::u_s_int& dt)override
	{
		unsigned char const* keys = SDL_GetKeyboardState(nullptr);
		if (keys[SDL_SCANCODE_UP])
		{
			Empaerior::Application::cur_state->get_camera().set_dimensions(Empaerior::Application::cur_state->get_camera().rect.w + 12, Empaerior::Application::cur_state->get_camera().rect.h + 10);
		}
		else if (keys[SDL_SCANCODE_DOWN])
		{
			Empaerior::Application::cur_state->get_camera().set_dimensions(Empaerior::Application::cur_state->get_camera().rect.w - 12, Empaerior::Application::cur_state->get_camera().rect.h - 10);
		}
		else if (keys[SDL_SCANCODE_W])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x, Empaerior::Application::cur_state->get_camera().rect.y - 10);
		}
		else if (keys[SDL_SCANCODE_S])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x, Empaerior::Application::cur_state->get_camera().rect.y + 10);
		}
		else if (keys[SDL_SCANCODE_A])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x - 10, Empaerior::Application::cur_state->get_camera().rect.y);
		}
		else if (keys[SDL_SCANCODE_D])
		{
			Empaerior::Application::cur_state->get_camera().set_position(Empaerior::Application::cur_state->get_camera().rect.x + 10, Empaerior::Application::cur_state->get_camera().rect.y);
		}

		f_timed_system->Update(ecs, dt);
		spr_system->update(ecs, dt);
	}
	
	virtual void Render() override//renders the state
	{
		
		spr_system->render(ecs,camera);
	
	}
	virtual void handleevents(const SDL_Event& event) override
	{
		event_system->handle_events(ecs, event);
	}

private:
	std::shared_ptr<Card_System> card_system;
	std::shared_ptr<Sprite_System> spr_system;
	std::shared_ptr<Event_System> event_system;
	std::shared_ptr<T_E_System> f_timed_system;
};

//a user defined application
class Test_Aplication : public Empaerior::Application
{
public:
	Test_Aplication()
	{
		window.Init("test", 960, 800);

		states.emplace_back(new APP_State());//add a new state

		set_state(states[0]);

		SDL_RenderSetLogicalSize(Application::window.renderer, Application::cur_state->get_camera().rect.w, Application::cur_state->get_camera().rect.h);

	}

	~Test_Aplication()
	{

	}

	//the main loop

	void run() override
	{
		Empaerior::u_s_int framestart = 0;
		Empaerior::u_s_int frametime = 0;
		Empaerior::u_s_int currentime = 0;
		Empaerior::u_s_int acumulator = 0;

		while (Empaerior::Application::is_running)
		{





	
			while (SDL_PollEvent(&Empaerior::Application::event)) {

				handlevents(Empaerior::Application::event);

			}
			if (!Empaerior::Application::is_paused)
			{


				framestart = SDL_GetTicks();
				frametime = framestart - currentime;

				if (frametime > 25) frametime = 25; //if too many frames are skipped

				currentime = framestart;
				acumulator += frametime;



				while (acumulator >= Empaerior::Application::dt)
				{


					//update 

					Update(Empaerior::Application::dt);

					acumulator -= Empaerior::Application::dt;



				}


				//Text_Sprite * norge = new Text_Sprite({ 0,0,200,200 }, "assets/font.ttf", 32 ,s, color);




				Empaerior::Application::window.clear();
				render();

				Empaerior::Application::window.render();


			}

			Empaerior::Asset_Loading::clean_textures();

		}
	}


	void handlevents(const SDL_Event& event) override
	{
		Empaerior::Application::window.window_listener.handleEvents(event);
		cur_state->handleevents(event);
	}
	void Update(const unsigned int& dt)override
	{
		cur_state->Update(Application::dt);
	}


	void render() override
	{
		cur_state->Render();
	}

	



};


Empaerior::Application* Empaerior::Create_Application()
{
	return new Test_Aplication();
}